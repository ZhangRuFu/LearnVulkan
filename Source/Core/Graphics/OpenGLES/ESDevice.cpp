#include "Core\Graphics\Mesh\Mesh.h"
#include "Core\Graphics\Mesh\VertexAttribDescriptor.h"
#include "Core\Graphics\Material.h"
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

void ESDevice::DrawMesh(const Mesh & mesh, const Material & material)
{
	VBOMap::iterator vboRes = m_meshToVBO.find(&mesh);
	if (vboRes == m_meshToVBO.end())
	{
		//create vertex attrib

		//add vbo

		//add vao
	}

	GLuint vbo = vboRes->second;
	GLuint vao = m_meshTOVAO.find(&mesh)->second;
	//GLuint ebo = ...

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	//glUseProgram()
	//glDrawElements()
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
