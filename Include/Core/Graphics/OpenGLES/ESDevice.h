#pragma once

#include "Core\Graphics\GfxDevice.h"
#include "EGL\egl.h"

/*
	OpenGLES Graphics API
*/

class ESDevice : public GfxDevice
{
private:
	EGLDisplay m_eglDisplay;

	EGLint m_majorVersion;
	EGLint m_minorVersion;

public:
	virtual void Init();
};