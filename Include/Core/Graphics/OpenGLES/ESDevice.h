#pragma once

#include "Core\Graphics\GfxDevice.h"
#include "EGL\egl.h"
#include "EGL\eglext.h"

/*
	OpenGLES Graphics API
*/

class ESDevice : public GfxDevice
{
private:
	EGLDisplay m_eglDisplay;
	EGLConfig m_eglConfig;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;

	EGLNativeWindowType m_nativeWindowType;

	EGLint m_majorVersion;
	EGLint m_minorVersion;

	static const EGLint m_redSize = 8;
	static const EGLint m_greenSize = 8;
	static const EGLint m_blueSize = 8;
	static const EGLint m_alphaSize = 8;
	static const EGLint m_bufferSize = m_redSize + m_greenSize + m_blueSize + m_alphaSize;
	static const EGLint m_colorBufferType = EGL_RGB_BUFFER;
	static const EGLint m_stencilSize = 8;
	static const EGLint m_depthSize = 24;
	static const EGLint m_renderableType = EGL_OPENGL_ES3_BIT_KHR;
	static const EGLint m_surfaceType = EGL_WINDOW_BIT;


public:
	ESDevice(EGLNativeWindowType nativeWindowType) { m_nativeWindowType = nativeWindowType; }

	virtual void Init();
	virtual void Destroy();
	virtual void SwapBuffer();
};