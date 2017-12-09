#ifndef __FILEMANAGER__
#define __FILEMANAGER__

QIMP_BEGIN_NAMESPACE

class FileReader
{
public:
	virtual ~FileReader() {};

	template<typename T>
	T Read()
	{
		T result;
		Read(&result, sizeof(T));
		return *((T*)(&result));
	};

	string ReadString()
	{
		uint32 size = Read<uint32>();
		string result(size, 0);
		Read(&result[0], size);
		return result;
	};

	virtual uint8*			GetPointer() = 0;
	virtual void			Read(void* data, int32 size) = 0;
	virtual int32			Size() = 0;
	virtual int32			Tell() = 0;
	virtual void			Seek(int32 pos) = 0;
	virtual bool			IsEof() = 0;
};

class MemoryReader : public FileReader
{
public:
	MemoryReader(uint8* data, int32 size);
	virtual ~MemoryReader();

	virtual uint8*			GetPointer()									{ return m_Bufffer; };
	virtual void			Read(void* data, int32 size);
	virtual int32			Size()											{ return m_BufferSize; };
	virtual int32			Tell()											{ return m_CurrentPos; };
	virtual void			Seek(int32 pos)									{ Assert(pos <= m_BufferSize); if (pos <= m_BufferSize) m_CurrentPos = pos; };
	virtual bool			IsEof()											{ return m_CurrentPos >= m_BufferSize; };


private:
	uint8*					m_Bufffer;
	int32					m_BufferSize;
	int32					m_CurrentPos;
};

class FileWriter
{
public:
	virtual ~FileWriter() {};

	virtual int32			Tell() = 0;
	virtual void			Seek(int32 pos) = 0;
	virtual void			Write(void* data, int32 size) = 0;
	virtual void			Flush() = 0;
};

class FileManager
{
public:
	typedef	std::list<std::string>	Strings;

protected:
	Strings					m_Paths;
	string					m_RootPath;
	umap<string, string>	m_FullPathCache;
	bool					m_IsEnablePathCache;

public:
	FileManager();
	~FileManager();
	FileReader*				CreateReader(const char* file);
	FileWriter*				CreateWriter(const char* file, bool append = true);

	/************************************************************************/
	/* 文件夹相关处理函数                                                   */
	/************************************************************************/
	void					AddPath(const char* path);
	string					GetWritablePath();
	bool					IsAbsolutePath(const char* file);
	bool					CreateDir(const char* szDir);
	bool					RemoveDir(const char* szDir);
	void					EnablePathCache(bool isEnable);
	void					ClearPathCache();

	/************************************************************************/
	/* 普通文件相关处理函数                                                 */
	/************************************************************************/
	string					GetFullPath(const char* file);
	bool					IsFileExist(const char* file);
	int32					GetFileSize(const char* file);
	unsigned char*			GetFileData(const char* szFileName, unsigned long* pSize, bool bDecrypt = true);
	string					GetFileString(const char* szFileName);
	string					GetFullPathForFileName(const char *szFile);
	bool					SaveFileData(const char* szFile, unsigned char* szData, unsigned long nSize, const char* szMode = "wb");
	bool					RenameFile(const char* szFile, const char* szNew);
	bool					RemoveFile(const char* szFile);

};

QIMP_END_NAMESPACE


#endif