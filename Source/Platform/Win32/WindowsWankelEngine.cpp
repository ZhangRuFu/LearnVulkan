#include "Platform\Win32\WindowsWankelEngine.h"
#include "Platform\Win32\WindowsConsoleLogHandler.h"
#include "Core\Log\LogManager.h"
#include "Core\Graphics\OpenGLES\ESDevice.h"

#include "../Source/Core/WankelEngine.cpp"

void WindowsWankelEngine::OnInit(void)
{
	WankelEngine::OnInit();

	//Log Handle
	m_logManager->SetLogHandler(new WindowsConsoleLogHandler());

	//GfxDevice Set Adreno OpenGL ES
	//m_gfxDevice = new ESDevice();
	//m_gfxDevice->Init();

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