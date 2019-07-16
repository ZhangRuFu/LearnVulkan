#include "Core\Graphics\OpenGLES\ESDevice.h"
#include "Core\Log\Debug.h"
#include <Windows.h>

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

}