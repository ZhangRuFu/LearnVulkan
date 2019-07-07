#pragma once

#include "Misc\Singleton.h"

class EngineLoop;
class LogManager;
class GfxDevice;

/*
 *
 *	WankelEngine Core
 *	
*/
#define NotifyEngineInitSuccess AfterInit();

template<class T>
class WankelEngine : public Singleton<T>
{
protected:
	EngineLoop* m_engineLoop;
	LogManager* m_logManager;
	GfxDevice* m_gfxDevice;

public:
	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();

protected:
	void AfterInit(void);	//After derived engine init all success
};
