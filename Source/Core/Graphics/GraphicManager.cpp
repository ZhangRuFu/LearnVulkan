#include "Core\Graphics\GraphicManager.h"

void GraphicManager::OnInit()
{
	//do nothing ?
}

void GraphicManager::OnDestroy()
{
	m_gfxDevice->Destroy();
}

void GraphicManager::SetGfxDevice(GfxDevice * gfxDevice)
{
	if (m_gfxDevice == nullptr && gfxDevice != nullptr)
		m_gfxDevice = gfxDevice;
}

void GraphicManager::DrawMesh(const Mesh & mesh, const Material & material)
{
	


}
