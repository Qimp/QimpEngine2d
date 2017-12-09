#include "common/Qimp.h"
#include "common/Win32.h"
#include "file/FileManager.h"


QIMP_BEGIN_NAMESPACE

void FileManager::EnablePathCache(bool isEnable)
{
	m_IsEnablePathCache = isEnable;
	if (isEnable)
	{
		ClearPathCache();
	}
}

void FileManager::ClearPathCache()
{
	m_FullPathCache.clear();
}

string FileManager::GetFullPath(const char* file)
{
	if (IsAbsolutePath(file))
		return file;
	string strFile = file;
	string fullPath = "";
	std::replace(strFile.begin(), strFile.end(), '\\', '/');
	if (m_IsEnablePathCache)
	{
		umap<string, string>::iterator cacheIter = m_FullPathCache.find(file);
		if (cacheIter != m_FullPathCache.end())
		{
			fullPath = cacheIter->second.c_str();
		}
	}
	if (fullPath.length() > 0)
		return fullPath;
	Strings::iterator iter = m_Paths.begin();
	for (; iter != m_Paths.end(); iter++)
	{
		fullPath = *iter;
		if (fullPath.length() > 0)
		{
			string strEnd = fullPath.substr(fullPath.length() - 1);
			if (strEnd != "/" && strEnd != "\\")
				fullPath += "/";
		}
		fullPath += strFile;
		fullPath = GetFullPathForFileName(fullPath.c_str());
		if (fullPath.length() > 0)
		{
			m_FullPathCache.insert(std::pair<string, string>(file, fullPath));
			return fullPath;
		}
	}
	return strFile;
}
string FileManager::GetFullPathForFileName(const char *szFile)
{
	if (szFile == NULL)
		return "";
	if (IsFileExist(szFile))
		return szFile;
	return "";
}

bool FileManager::SaveFileData(const char* szFile, unsigned char* szData, unsigned long nSize, const char* szMode)
{
	string strFullPath = IsAbsolutePath(szFile) ? szFile : GetWritablePath() + szFile;
	FILE* fp = 0;
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
	errno_t err = fopen_s(&fp, strFullPath.c_str(), szMode);
	if (!fp || err) {
#else
	fp = fopen(strFullPath.c_str(), szMode);
	if (!fp) {
#endif
		return false;
	}
	fwrite(szData, 1, nSize, fp);
	fclose(fp);
	return true;
}

FileReader* FileManager::CreateReader(const char* name)
{
	unsigned long size = 0;
	uint8* pData = (uint8*)GetFileData(name, &size);
	if (pData == NULL) return NULL;
	FileReader * pReader = new MemoryReader(pData, size);
	return pReader;
}

bool FileManager::RenameFile(const char* szFile, const char* szNew)
{
	string strFile = IsAbsolutePath(szFile) ? szFile : GetWritablePath() + szFile;
	if (!IsFileExist(strFile.c_str()))
		return false;
	string strNew;
	if (IsAbsolutePath(szNew))
		strNew = szNew;
	else
		strNew = GetWritablePath() + szNew;
	// 返回值 执行成功则返回0，失败返回-1，错误原因存于errno
	int ret = rename(strFile.c_str(), strNew.c_str());
	if (ret != 0)
		printf("%d\n", errno);
	return ret == 0;
}

bool FileManager::RemoveFile(const char* szFile)
{
	string strFile = IsAbsolutePath(szFile) ? szFile : GetWritablePath() + szFile;
	// 成功则返回0，失败则返回-1，错误原因存于errno。
	return remove(strFile.c_str()) == 0;
}

bool FileManager::CreateDir(const char* szDir)
{
	string strDir = IsAbsolutePath(szDir) ? szDir : GetWritablePath() + szDir;
	std::replace(strDir.begin(), strDir.end(), '\\', '/');
	int nFind = 0;
	while (nFind != string::npos)
	{
		nFind = strDir.find('/', nFind + 1);
		string strCur = strDir.substr(0, nFind);
		BOOL ret = CreateDirectoryA(strCur.c_str(), NULL);
		if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
			return false;
	}
	return true;
}

BOOL IsDirectory(const char *szDir)
{
	char szCurPath[MAX_PATH];
	ZeroMemory(szCurPath, MAX_PATH);
	sprintf_s(szCurPath, MAX_PATH, "%s//*", szDir);
	WIN32_FIND_DATAA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
		return FALSE;
	}
	else
	{
		FindClose(hFile);
		return TRUE;
	}
}

bool RemoveDirectory(const char* szDir)
{
	char szCurPath[MAX_PATH];
	_snprintf(szCurPath, MAX_PATH, "%s//*.*", szDir);
	WIN32_FIND_DATAA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
	HANDLE hFile = FindFirstFileA(szCurPath, &FindFileData);
	BOOL bIsFinded = TRUE;
	while (bIsFinded)
	{
		bIsFinded = FindNextFileA(hFile, &FindFileData);
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, ".."))
		{
			string strFileName = "";
			strFileName = strFileName + szDir + "//" + FindFileData.cFileName;
			if (IsDirectory(strFileName.c_str()))
				RemoveDirectory(strFileName.c_str());
			else
				DeleteFileA(strFileName.c_str());
		}
	}
	FindClose(hFile);
	return RemoveDirectoryA(szDir) != 0;
}

bool FileManager::RemoveDir(const char* szDir)
{
	string strDir = IsAbsolutePath(szDir) ? szDir : GetWritablePath() + szDir;
	return RemoveDirectory(strDir.c_str());
}

string FileManager::GetFileString(const char* szFileName)
{
	string strRet;
	if (!IsFileExist(szFileName))
		return strRet;
	unsigned long nSize = 0;
	unsigned char* szData = GetFileData(szFileName, &nSize);
	if (szData)
	{
		strRet = (const char*)szData;
		delete szData;
		szData = nullptr;
	}
	return strRet;
}



using namespace std;

string Utf8ToGbk(const char *src)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (strTemp.find('?') != string::npos)
	{
		strTemp.assign(src);
	}
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}

static int32 FileSize(FILE* handle)
{
	int32 pos = ftell(handle);
	fseek(handle, 0, SEEK_END);
	int32 size = (int32)ftell(handle);
	fseek(handle, pos, SEEK_SET);
	return size;
}

FileManager::FileManager()
{
	char szRootPath[MAX_PATH] = { 0 };
	WCHAR wszRootPath[MAX_PATH] = { 0 };
	int nNum = WideCharToMultiByte(CP_ACP, 0, wszRootPath, GetCurrentDirectoryW(MAX_PATH, wszRootPath), szRootPath, MAX_PATH, NULL, NULL);
	std::replace(&szRootPath[0], &szRootPath[nNum - 1], '\\', '/');
	szRootPath[nNum] = '/';
	m_RootPath = szRootPath;

	m_FullPathCache.clear();
	m_IsEnablePathCache = true;
}

FileManager::~FileManager()
{

}

string FileManager::GetWritablePath()
{
	return string("./");
}

void FileManager::AddPath(const char* path)
{
	if (IsAbsolutePath(path))
		m_Paths.push_front(Utf8ToGbk(path));
	else
		m_Paths.push_front(m_RootPath + Utf8ToGbk(path));
}

FileWriter* FileManager::CreateWriter(const char* file, bool append)
{
	return NULL;
}

int32 FileManager::GetFileSize(const char* file)
{
	string strFile = Utf8ToGbk(file);
	string strFullPath = GetFullPath(strFile.c_str());
	int32 size = 0;
	FILE* fp = fopen(strFullPath.c_str(), "rb");
	if (fp)
	{
		size = FileSize(fp);
		fclose(fp);
	}
	return size;
}

bool FileManager::IsAbsolutePath(const char* file)
{
	string strPathAscii = Utf8ToGbk(file);
	if (strPathAscii.length() > 2
		&& ((strPathAscii[0] >= 'a' && strPathAscii[0] <= 'z') || (strPathAscii[0] >= 'A' && strPathAscii[0] <= 'Z'))
		&& strPathAscii[1] == ':')
	{
		return true;
	}
	return false;
}

bool FileManager::IsFileExist(const char* file)
{
	string strFile = Utf8ToGbk(file);
	if (strFile.length() == 0)
		return false;
	string strFullPath = GetFullPath(file);
	if (strFullPath.length() == 0)
		return false;
	return GetFileAttributesA(strFullPath.c_str()) != -1 ? true : false;
}


static umap<string, bool> s_mapCheckPathMatchCase;
void checkPathMatchCase(const char* szFileName, const char* szFullPath)
{
	string strFullPath(szFullPath);
	std::replace(strFullPath.begin(), strFullPath.end(), '\\', '/');
	int nCur = strFullPath.find("/") + 1;
	while (true)
	{
		int nFind = strFullPath.find("/", nCur);
		bool bIsBreak = nFind == string::npos;
		string strName = bIsBreak ? strFullPath.substr(nCur) : strFullPath.substr(nCur, nFind - nCur);
		string strCur = bIsBreak ? strFullPath : strFullPath.substr(0, nFind);
		if (strName == "." || strName == "..")
		{
			nCur = nFind + 1;
			continue;
		}
		umap<string, bool>::iterator itr = s_mapCheckPathMatchCase.find(strCur);
		if (itr == s_mapCheckPathMatchCase.end())
		{
			WIN32_FIND_DATAA FindFileData;
			ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATAA));
			HANDLE hFile = FindFirstFileA(strCur.c_str(), &FindFileData);
			if (hFile == INVALID_HANDLE_VALUE)
				break;
			FindClose(hFile);
			if (strName == FindFileData.cFileName)
				s_mapCheckPathMatchCase[strCur] = true;
			else
				QIMP_LOGL(kLogLevelError, "match case error: %s -> %s", szFileName, FindFileData.cFileName);
		}
		if (bIsBreak) break;
		nCur = nFind + 1;
	}
}

unsigned char* FileManager::GetFileData(const char* szFileName, unsigned long * pSize, bool bDecrypt /* = true */)
{
	QIMP_ASSERT(szFileName != NULL, "FileManager::GetFileData #1(szFileName) is NULL");
	QIMP_ASSERT(pSize != NULL, "FileManager::GetFileData #2(szFileName) is NULL");
	*pSize = 0;
	unsigned char * pBuff = NULL;

	string strFullPath = GetFullPath(szFileName);
	checkPathMatchCase(szFileName, strFullPath.c_str());
	FILE* fp = fopen(strFullPath.c_str(), "rb");
	bool bNeedDecrypt = false;
	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		*pSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		pBuff = new unsigned char[*pSize + 1];
		*pSize = fread(pBuff, sizeof(unsigned char), *pSize, fp);
		pBuff[*pSize] = 0;
		fclose(fp);
	}
	else
		LogByLevel(kLogLevelError, "can't get file data: %s", szFileName);
	return pBuff;
}

//=====================================
MemoryReader::MemoryReader(uint8* buffer, int32 size)
	:m_Bufffer(buffer)
	, m_BufferSize(size)
	, m_CurrentPos(0)
{

}

MemoryReader::~MemoryReader()
{
	delete m_Bufffer;
}

void MemoryReader::Read(void* data, int32 size)
{
	memcpy(data, m_Bufffer + m_CurrentPos, size);
	m_CurrentPos += size;
}


QIMP_END_NAMESPACE
