

#include "Core\EngineLoop.h"
#include "Core\Log\Debug.h"

#include "Core\Graphics\Shader.h"
#include "Core\Graphics\Material.h"
#include "Core\Graphics\Mesh\Mesh.h"

#include "Core\Graphics\GraphicManager.h"

/*
	Empty Engine Loop
	For Test
*/
class EmptyEngineLoop : public EngineLoop
{
private:
	/* smart pointer */
	Material *m_mat;
	Mesh *m_mesh;

public:
	virtual void Init()
	{
		Debug::Log("Wankel Init");
		
		Shader *shader = new Shader("./Asset/Shader/SimpleMesh");
		m_mat = new Material(*shader);

		m_mesh = new Mesh(Mesh::MeshType::Cube);
	}

	virtual void Resize()
	{

	}

	virtual void Destroy()
	{
		delete m_mesh;

		/* TODO : shader ??*/
		delete m_mat;
	}

	virtual void Update()
	{

	}

	virtual void Render()
	{
		GraphicManager::Instance()->DrawMesh(*m_mesh, *m_mat);

	}
};

MakeEngineLoop(EmptyEngineLoop)