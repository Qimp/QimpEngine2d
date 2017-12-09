#include "common/Qimp.h"
#include "common/Win32.h"
#include "render/RenderManager.h"
#include "render/ShaderProgram.h"

QIMP_BEGIN_NAMESPACE

HDC	s_hDC = nullptr;

bool RenderManager::InitGL()
{
	// 初始化 OPENGL
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),						// size
		1,													// version
		PFD_SUPPORT_OPENGL |								// OpenGL window
		PFD_DRAW_TO_WINDOW |								// render to window
		PFD_DOUBLEBUFFER,									// support double-buffering
		PFD_TYPE_RGBA,										// color type
		32,													// preferred color depth
		0, 0, 0, 0, 0, 0,									// color bits (ignored)
		0,													// no alpha buffer
		0,													// alpha bits (ignored)
		0,													// no accumulation buffer
		0, 0, 0, 0,											// accum bits (ignored)
		0,													// depth buffer 这里不需要深度buff
		8,													// no stencil buffer
		0,													// no auxiliary buffers
		PFD_MAIN_PLANE,										// main layer
		0,													// reserved
		0, 0, 0,											// no layer, visible, damage masks
	};

	// 获取窗口DC
	s_hDC = ::GetDC(g_Win32->GetHWnd());

	// 检查我们设置的格式 dc是否支持
	int pixelFormat = ChoosePixelFormat(s_hDC, &pfd);
	if (!pixelFormat) 
	{
		QIMP_LOGL(kLogLevelError, "pixelFormat not support");
		return false;
	}
	// 支持那就设置
	SetPixelFormat(s_hDC, pixelFormat, &pfd);
	// 创建渲染上下文
	HGLRC hRC = wglCreateContext(s_hDC);
	if (!wglMakeCurrent(s_hDC, hRC))
	{
		QIMP_LOGL(kLogLevelError, "hRC bind fail");
		return false;
	}
	// 必不可少的初始化
	GLenum GlewInitResult = glewInit();
	GlewInitResult = GlewInitResult;
	if (GLEW_OK != GlewInitResult)
	{
		QIMP_LOGL(kLogLevelError, "glewInit Failed!");
		return false;
	}

	// 清屏时使用的颜色
	glClearColor(0.0f, 0.0f, 0.6f, 1.0f);

	return true;
}

void RenderManager::SetViewPort(uint32 x, uint32 y, uint32 w, uint32 h)
{
	Rect rect = Rect(x, y, w, h);
	if (m_Viewport != rect)
	{
		m_Viewport = rect;
		glViewport(rect.x, rect.y, rect.width, rect.height);
	}
}














void RenderManager::Render()
{
	static ShaderProgram temp;
	static GLuint s_Vao = 0;
	static int a = 0;
	if (a == 0)
	{
		a = 1;
		// Shaders
		const GLchar* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 position;\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
			"}\0";
		const GLchar* fragmentShaderSource = "#version 330 core\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"}\n\0";


		temp.InitShader(vertexShaderSource, fragmentShaderSource);
		temp.UseShaderProgram();


		GLfloat vertices[] = {
			0.5f, 0.5f, 0.0f,  // Top Right
			0.5f, -0.5f, 0.0f,  // Bottom Right
			-0.5f, -0.5f, 0.0f,  // Bottom Left
			-0.5f, 0.5f, 0.0f   // Top Left 
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3,  // First Triangle
			1, 2, 3   // Second Triangle
		};
		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);
		s_Vao = VAO;
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

		glBindVertexArray(0);
	}


	// 使用设置的背景颜色清空屏幕
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw our first triangle
	temp.UseShaderProgram();
	glBindVertexArray(s_Vao);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	SwapBuffers(s_hDC);
}


QIMP_END_NAMESPACE