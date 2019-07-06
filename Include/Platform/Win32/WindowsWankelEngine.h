#pragma once
#include "Core\WankelEngine.h"

/*
	WankelEngine in windows
*/

class WindowsWankelEngine : public WankelEngine<WindowsWankelEngine>
{
public:
	virtual void OnInit();
	virtual void OnDestroy();
	virtual void OnUpdate();
};