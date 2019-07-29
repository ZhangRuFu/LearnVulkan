#include "Core\Common.h"
#include "Core\WankelEngine.h"
#include "Core\EngineLoop.h"
#include "Core\Log\LogManager.h"
#include "Core\Graphics\GraphicManager.h"

template<class T>
void WankelEngine<T>::OnInit()
{
	m_engineLoop = GetEngineLoop();

	//Platform Independent Init
	LogManager::Init();
	m_logManager = LogManager::Instance();

	GraphicManager::Init();
	m_graphicManager = GraphicManager::Instance();
}

template<class T>
void WankelEngine<T>::OnDestroy()
{
	//Engine Loop first
	m_engineLoop->Destroy();
	
	//Platform Indentdent Destroy
	LogManager::Destroy();
	GraphicManager::Destroy();
}

template<class T>
void WankelEngine<T>::OnUpdate()
{
	//Platform Indentdent Update
	
	//Engine Loop
	m_engineLoop->Update();
	m_engineLoop->Render();
}

template<class T>
void WankelEngine<T>::AfterInit(void)
{
	m_engineLoop->Init();
}
