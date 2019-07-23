#pragma once

/*
	Í¼ÐÎAPI³éÏó
*/

class GfxDevice
{
public:
	virtual void Init(void) = 0;
	virtual void Destroy(void) = 0;

	//swap double buffer
	virtual void SwapBuffer() = 0;
};