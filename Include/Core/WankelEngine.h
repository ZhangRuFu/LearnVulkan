#pragma once

#include "Misc\Singleton.h"

class EngineLoop;
class LogManager;

/*
 *
 *	WankelEngine Core
 *	
*/

template<class T>
class WankelEngine : public Singleton<T>
{
protected:
	EngineLoop* m_engineLoop;
	LogManager* m_logManager;

public:
	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
};
