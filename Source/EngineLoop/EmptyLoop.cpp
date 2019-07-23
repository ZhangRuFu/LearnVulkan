#include "Core\EngineLoop.h"
#include "Core\Log\Debug.h"

/*
	Empty Engine Loop
	For Test
*/
class EmptyEngineLoop : public EngineLoop
{
public:
	virtual void Init()
	{
		Debug::Log("Wankel Init");
	}

	virtual void Resize()
	{

	}

	virtual void Destroy()
	{

	}

	virtual void Update()
	{

	}

	virtual void Render()
	{



	}
};

MakeEngineLoop(EmptyEngineLoop)