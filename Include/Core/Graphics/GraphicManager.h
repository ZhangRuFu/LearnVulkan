#pragma once
#include "Core\Misc\Singleton.h"
#include "Core\Graphics\GfxDevice.h"

class Mesh;
class Material;

/*
	绘制接口封装
*/
class GraphicManager : public Singleton<GraphicManager>
{
private:
	GfxDevice* m_gfxDevice;

public:
	virtual void OnInit();
	virtual void OnDestroy();

public:
	void SetGfxDevice(GfxDevice* gfxDevice);

	void DrawMesh(const Mesh& mesh, const Material& material);
};
