#include "Core\WankelEngine.h"
#include "Core\EngineLoop.h"
#include "Core\Log\LogManager.h"

template<class T>
void WankelEngine<T>::OnInit()
{
	//Platform Independent Init
	LogManager::Init();
	m_logManager = LogManager::Instance();

	//Engine Loop
	m_engineLoop->Update();
}

template<class T>
void WankelEngine<T>::OnDestroy()
{
	//Engine Loop first
	m_engineLoop->Destroy();

	//Platform Indentdent Destroy
	LogManager::Destroy();
	
}

template<class T>
void WankelEngine<T>::OnUpdate()
{
	//Platform Indentdent Update

	//Engine Loop
	m_engineLoop->Update();
	m_engineLoop->Render();
}
