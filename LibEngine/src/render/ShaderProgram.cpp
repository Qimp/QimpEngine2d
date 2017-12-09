#include "common/Qimp.h"
#include "common/Win32.h"
#include "render/ShaderProgram.h"

QIMP_BEGIN_NAMESPACE

ShaderProgram::ShaderProgram()
: m_ShaderProgram(0)
{

}

bool ShaderProgram::InitShader(const char* vsStr, const char* psStr)
{

	GLuint vsId = CompileShder(vsStr, GL_VERTEX_SHADER);
	if (vsId == 0)
		return false;

	GLuint psId = CompileShder(psStr, GL_FRAGMENT_SHADER);
	if (psId == 0)
	{
		glDeleteShader(vsId);
		return false;
	}
	
	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vsId);
	glAttachShader(m_ShaderProgram, psId);

	glLinkProgram(m_ShaderProgram);
	glDeleteShader(vsId);
	glDeleteShader(psId);

	if (!CheckLink(m_ShaderProgram))
	{
		glDeleteProgram(m_ShaderProgram);
		m_ShaderProgram = 0;
		return false;
	}
	
	return true;
}

GLuint ShaderProgram::CompileShder(const char* source, GLenum type)
{
	GLuint shaderId = glCreateShader(type);
	if (shaderId == 0)
		return 0;

	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	if (!CheckCompile(shaderId))
	{
		glDeleteShader(shaderId);
		return 0;
	}
	return shaderId;
}

bool ShaderProgram::CheckCompile(uint32 shaderId)
{
	//获取编译状态
	GLint compiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);

	//编译失败时，获取Log信息
	if (!compiled)
	{
		//获取Log长度
		GLint infoLen;
		glGetShaderiv(shaderId,GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			char* pLogMsg = new char[infoLen];
			//获取编译失败时的Log信息
			glGetShaderInfoLog(shaderId, infoLen, NULL, pLogMsg);
			QIMP_LOGL(kLogLevelWarning, "Error compiling shader: \n%s\n", pLogMsg);
			delete[] pLogMsg;
		}
		return false;
	}
	return true;
}

bool ShaderProgram::CheckLink(uint32 programId)
{
	//获取链接状态
	GLint Success;
	glGetProgramiv(programId, GL_LINK_STATUS, &Success);

	if (!Success)
	{
		//获取Log长度
		GLint infoLen;
		glGetProgramiv(programId,GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 0)
		{
			char* pLogMsg = new char[infoLen];
			//获取链接失败时的Log信息
			glGetProgramInfoLog(programId, infoLen, NULL, pLogMsg);
			QIMP_LOGL(kLogLevelWarning, "Error linking shader program: \n%s\n", pLogMsg);
			delete[] pLogMsg;
		}
		return false;
	}
	return true;
}

void ShaderProgram::UseShaderProgram()
{
	if (m_ShaderProgram)
		glUseProgram(m_ShaderProgram);
}

ShaderProgram::~ShaderProgram()
{
	if (m_ShaderProgram)
		glDeleteProgram(m_ShaderProgram);
}



QIMP_END_NAMESPACE
