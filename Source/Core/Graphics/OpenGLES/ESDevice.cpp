#include "Core\Graphics\Mesh\Mesh.h"
#include "Core\Graphics\Mesh\VertexAttribGenerator.h"
#include "Core\Graphics\Material.h"
#include "Core\Graphics\Shader.h"
#include "Core\Graphics\OpenGLES\ESDevice.h"
#include "Core\Log\Debug.h"

void ESDevice::Init()
{
	eglBindAPI(EGL_OPENGL_ES_API);

	m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (m_eglDisplay == EGL_NO_DISPLAY)
	{
		Debug::Error("Can not get EGL Display");
		return;
	}

	EGLBoolean res = eglInitialize(m_eglDisplay, &m_majorVersion, &m_minorVersion);
	if (res == EGL_FALSE)
	{
		Debug::Error("Initialize EGL failed");
		return;
	}

	Debug::Log(StringUtil::format("EGL Version: {}.{}", m_majorVersion, m_minorVersion));

	EGLint maxConfig;
	res = eglGetConfigs(m_eglDisplay, nullptr, 0, &maxConfig);
	if (res == EGL_FALSE)
	{
		Debug::Error("Get EGL Config Max Count failed");
		return;
	}

	Debug::Log(StringUtil::format("EGL Config Count : {}", maxConfig));

#ifdef _LOG_ALL_EGLCONFIG
	EGLConfig* eglConfigs = new EGLConfig[maxConfig]{};
	res = eglGetConfigs(m_eglDisplay, eglConfigs, maxConfig, &maxConfig);
	if (res == EGL_FALSE)
	{
		EGLint error = eglGetError();
		Debug::Error("Get EGL Config failed");
		return;
	}

	for (int i = 0; i < maxConfig; ++i)
	{
		EGLint attribValue = 0;
		Debug::Log(StringUtil::format("EGLConfig : {}", i));

		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_RED_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Red Size : {}", attribValue));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_GREEN_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Green Size : {}", attribValue));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_BLUE_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Blue Size : {}", attribValue));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_ALPHA_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Alpha Size : {}", attribValue));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_BUFFER_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Bit Per Pixel Size : {}", attribValue));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_STENCIL_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Stencil Size : {}", attribValue));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_DEPTH_SIZE, &attribValue);
		Debug::Log(StringUtil::format("Depth Size : {}", attribValue));

		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_RENDERABLE_TYPE, &attribValue);
		Debug::Log(StringUtil::format("Support Renderable Type : ES1.x-{} ES2-{} ES3-{}", (attribValue & EGL_OPENGL_ES_BIT) != 0, (attribValue & EGL_OPENGL_ES2_BIT) != 0, (attribValue & EGL_OPENGL_ES3_BIT_KHR) != 0));
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_SURFACE_TYPE, &attribValue);
		Debug::Log(StringUtil::format("Support Widnow Surface : {}", attribValue & EGL_WINDOW_BIT));

		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_CONFIG_CAVEAT, &attribValue);
		Debug::Log(StringUtil::format("Slow Device :  {}, Compatible Device {}", (attribValue & EGL_SLOW_CONFIG) != 0, (attribValue & EGL_NON_CONFORMANT_CONFIG) != 0));
		
		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_MIN_SWAP_INTERVAL, &attribValue);
		Debug::Log(StringUtil::format("Min Swap Interval : {}", attribValue));

		eglGetConfigAttrib(m_eglDisplay, eglConfigs[i], EGL_MAX_SWAP_INTERVAL, &attribValue);
		Debug::Log(StringUtil::format("Max Swap Interval : {}", attribValue));
	}
	delete eglConfigs;
#endif
	
	//Choose need config
	EGLint needConfig[] = {
		EGL_COLOR_BUFFER_TYPE, m_colorBufferType,
		EGL_BUFFER_SIZE, m_bufferSize,
		EGL_RED_SIZE, m_redSize,
		EGL_GREEN_SIZE, m_greenSize,
		EGL_BLUE_SIZE, m_blueSize,
		EGL_ALPHA_SIZE, m_alphaSize,
		EGL_STENCIL_SIZE, m_stencilSize,
		EGL_DEPTH_SIZE, m_depthSize,
		EGL_RENDERABLE_TYPE, m_renderableType,
		EGL_SURFACE_TYPE, m_surfaceType,
		EGL_NONE
	};
	
	EGLint numConfig;
	res = eglChooseConfig(m_eglDisplay, needConfig, &m_eglConfig, 1, &numConfig);
	if (res == EGL_FALSE)
	{
		Debug::Error("Choose Config Failed");
		return;
	}
	
	EGLint surfaceAttribList[] = { EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE };
	m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_nativeWindowType, surfaceAttribList);
	if (m_eglSurface == EGL_NO_SURFACE)
	{
		Debug::Error("EGL Create Surface Failed");
		return;
	}

	EGLint contextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
	m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttribList);
	if (m_eglContext == EGL_NO_CONTEXT)
	{
		Debug::Error("EGL Create Context Failed");
		return;
	}

	res = eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
	if (res == EGL_FALSE)
	{
		Debug::Error("EGL Bind Context Failed");
		eglDestroySurface(m_eglDisplay, m_eglSurface);
		eglDestroyContext(m_eglDisplay, m_eglContext);
		return;
	}

	Debug::Log("EGL Init Success");
}

void ESDevice::Destroy(void)
{
	eglDestroyContext(m_eglDisplay, m_eglContext);
	eglDestroySurface(m_eglDisplay, m_eglSurface);

	Debug::Log("ESDevice Destroy");
}

void ESDevice::SwapBuffer(void)
{
	/*TODO : Swap Interval ?*/
	//eglSwapInterval(m_eglDisplay, 1);
	eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

void ESDevice::Clear()
{
	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ESDevice::DrawMesh(const Mesh & mesh, const Material & material)
{
	GLuint resVBO = 0;
	GLuint resVAO = 0;
	GLuint resEBO = 0;

	VBOMap::iterator vboRes = m_meshToVBO.find(&mesh);
	if (vboRes == m_meshToVBO.end())
	{
		//create vertex attrib
		std::vector<VertexAttribGenerator::VertexAttribution> vas;
		VertexAttribGenerator::Generate(mesh, vas);

		//vbo
		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		int stride = 0;

		for (int i = 0; i < vas.size(); ++i)
			stride += vas[i].m_size;

		char* buffer = new char[stride * mesh.GetVertexCount()];
		char* bufferPointer = buffer;

		const std::vector<Vertex>& meshVertexs = mesh.GetVertex();

		for (int i = 0; i < meshVertexs.size(); ++i)
		{
			memcpy(bufferPointer, &(meshVertexs[i].m_position), 3 * sizeof(float));
			bufferPointer += 3 * sizeof(float);
			memcpy(bufferPointer, &(meshVertexs[i].m_normal), 3 * sizeof(float));
			bufferPointer += 3 * sizeof(float);
			memcpy(bufferPointer, &(meshVertexs[i].m_texCoord), 2 * sizeof(float));
			bufferPointer += 2 * sizeof(float);
		}

		/* TODO : start IO ??? */
		glBufferData(GL_ARRAY_BUFFER, stride * mesh.GetVertexCount(), buffer, GL_STATIC_DRAW);
		
		
		delete[] buffer;
		bufferPointer = nullptr;

		//vao
		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		int offset = 0;

		//type transfer
		GLenum typeTrans[]{ GL_UNSIGNED_BYTE, GL_BYTE, GL_HALF_FLOAT, GL_FLOAT };

		for (int i = 0; i < vas.size(); ++i)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, vas[i].m_componentSize, typeTrans[(int)vas[i].m_attribType], false, stride, (void*)offset);
			offset += vas[i].m_size;
		}
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		//ebo
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.GetIndexCount(), mesh.GetIndex().data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//add
		m_meshToVBO[&mesh] = vbo;
		m_meshToVAO[&mesh] = vao;
		m_meshToEBO[&mesh] = ebo;

		resVBO = vbo;
		resVAO = vao;
		resEBO = ebo;
	}
	else
	{
		resVBO = vboRes->second;
		resVAO = m_meshToVAO.find(&mesh)->second;
		resEBO = m_meshToEBO.find(&mesh)->second;
	}

	//shader
	ShaderMap::iterator shaderRes = m_shaderMap.find(&material.GetShader());
	GLuint shaderID = -1;
	if (shaderRes == m_shaderMap.end())
	{
		//create shader
		const Shader& shader = material.GetShader();

		GLuint programID = CreateShader(shader);
		if (programID == -1)
			return;

		//add shader
		m_shaderMap[&shader] = programID;
		shaderID = programID;
	}
	else
		shaderID = shaderRes->second;

	glBindVertexArray(resVAO);
	
	glUseProgram(shaderID);
	glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, mesh.GetVertexCount());

	glBindVertexArray(0);
	glUseProgram(0);
}

GLuint ESDevice::CreateShader(const Shader & shader)
{
	GLuint shaderID = -1;
	
	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);

	const String& vsSource = shader.GetVertexShaderSource();
	const char *source = vsSource.data();
	GLint len = vsSource.size();
	glShaderSource(vsID, 1, &source, &len);
	glCompileShader(vsID);
	
	GLint isSuccess = 0;
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		GLint infoLogLen = 0;
		glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &infoLogLen);
		/* TODO : Cache buffer */
		GLchar *infoBuffer = new GLchar[infoLogLen]();
		glGetShaderInfoLog(vsID, infoLogLen, &infoLogLen, infoBuffer);
		Debug::Error(StringUtil::format("[{0}] vertex shader compile error : {1}", shader.GetShaderPath(), infoBuffer));
		
		delete[] infoBuffer;
		glDeleteShader(vsID);
		return -1;
	}
	
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);
	const String& fsSource = shader.GetFragmentShaderSource();
	source = fsSource.data();
	len = fsSource.size();
	glShaderSource(fsID, 1, &source, &len);
	glCompileShader(fsID);

	glGetShaderiv(fsID, GL_COMPILE_STATUS, &isSuccess);
	if (!isSuccess)
	{
		GLint infoLogLen = 0;
		glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &infoLogLen);
		GLchar *infoBuffer = new GLchar[infoLogLen]();
		glGetShaderInfoLog(fsID, infoLogLen, nullptr, infoBuffer);
		Debug::Error(StringUtil::format("[{0}] fragment shader compile error : {1}", shader.GetShaderPath(), infoBuffer));

		delete[] infoBuffer;
		glDeleteShader(fsID);
		return -1;
	}

	//vs and fs is ok
	GLuint program = glCreateProgram();
	glAttachShader(program, vsID);
	glAttachShader(program, fsID);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &isSuccess);
	if (!isSuccess)
	{
		GLint infoLogLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
		GLchar *infoBuffer = new GLchar[infoLogLen]();
		glGetProgramInfoLog(program, infoLogLen, nullptr, infoBuffer);
		Debug::Error(StringUtil::format("{0} shader program compile error : {1}", shader.GetShaderPath(), infoBuffer));

		delete[] infoBuffer;
		glDeleteProgram(program);
		return -1;
	}

	//link success
	//print shader info
	Debug::Log(StringUtil::format("Shader : {0}, Information :", shader.GetShaderPath()));
	Debug::Log("-------------------------------");
	
	GLint programArg = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &programArg);
	Debug::Log(StringUtil::format("Active Attributes Count : {0}", programArg));
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &programArg);
	Debug::Log(StringUtil::format("Active Attributes Max Length : {0}", programArg));

	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &programArg);
	Debug::Log(StringUtil::format("Active Uniforms Count : {0}", program));
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &programArg);
	Debug::Log(StringUtil::format("Active Uniforms Max Length : {0}", programArg));

	glGetProgramiv(program, GL_ATTACHED_SHADERS, &programArg);
	Debug::Log(StringUtil::format("Attach Shaders Count : {0}", programArg));

	//glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &programArg);
	//Debug::Log(StringUtil::format("Program Binary Length : {0}", programArg));

	Debug::Log("-------------------------------");

	return program;
}
