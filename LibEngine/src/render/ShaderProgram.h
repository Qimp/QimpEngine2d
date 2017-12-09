#ifndef __SHADERPROGRAM__H__
#define __SHADERPROGRAM__H__

QIMP_BEGIN_NAMESPACE

class ShaderProgram
{
private:
	uint32				m_ShaderProgram;

public:
	ShaderProgram();
	~ShaderProgram();

	uint32				GetHandle() { return m_ShaderProgram; };
	bool				InitShader(const char* vsStr, const char* psStr);
	void				UseShaderProgram();

private:
	GLuint				CompileShder(const char* source, GLenum type);
	bool				CheckCompile(uint32 shaderId);
	bool				CheckLink(uint32 programId);
};

QIMP_END_NAMESPACE

#endif