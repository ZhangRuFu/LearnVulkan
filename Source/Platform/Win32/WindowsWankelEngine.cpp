#include "Platform\Win32\WindowsWankelEngine.h"
#include "Platform\Win32\WindowsConsoleLogHandler.h"
#include "Core\Log\LogManager.h"

void WindowsWankelEngine::OnInit(void)
{
	WankelEngine::OnInit();
	m_logManager->SetLogHandler(new WindowsConsoleLogHandler());
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