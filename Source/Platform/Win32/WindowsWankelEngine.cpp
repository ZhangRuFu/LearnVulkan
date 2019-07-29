#include "Platform\Win32\WindowsWankelEngine.h"
#include "Platform\Win32\WindowsConsoleLogHandler.h"
#include "Core\Log\LogManager.h"
#include "Core\Graphics\OpenGLES\ESDevice.h"
#include "Platform\Win32\WindowsCommon.h"

#include "../Source/Core/WankelEngine.cpp"

void WindowsWankelEngine::OnInit(void)
{
	WankelEngine::OnInit();

	//Log Handle
	m_logManager->SetLogHandler(new WindowsConsoleLogHandler());

	//GfxDevice Set Mali OpenGL ES
	m_graphicManager->SetGfxDevice(new ESDevice(gHWND));

	NotifyEngineInitSuccess
}

void WindowsWankelEngine::OnDestroy(void)
{
	//Do self destroy
	WankelEngine::OnDestroy();
}

void WindowsWankelEngine::OnUpdate(void)
{
	WankelEngine::OnUpdate();
}