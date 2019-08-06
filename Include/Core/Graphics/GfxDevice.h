#pragma once

/*
	Í¼ÐÎAPI³éÏó
*/

class Mesh;
class Material;

class GfxDevice
{
public:
	virtual void Init(void) = 0;
	virtual void Destroy(void) = 0;
	virtual void Clear(void) = 0;

	//swap double buffer
	virtual void SwapBuffer(void) = 0;

	virtual void DrawMesh(const Mesh &mesh, const Material& material) = 0;
};