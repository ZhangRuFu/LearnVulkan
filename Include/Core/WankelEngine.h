#pragma once

#include "Misc\Singleton.h"

/*
 *
 *	转子引擎的核心
 *	
*/

class WankelEngine : public Singleton<WankelEngine>
{
public:
	//TODO : C++11 有对 override 这种方式的支持嘛?
	virtual void OnInit()
	{
		//Init Manager
	}

	virtual void OnDestroy()
	{
		//Destroy Manager
	}

//To Platform Dependant
public:
	void Update();	//Tick
};
