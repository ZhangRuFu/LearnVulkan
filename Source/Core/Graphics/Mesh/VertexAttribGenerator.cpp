#include "Core\Graphics\Mesh\VertexAttribGenerator.h"
#include "Core\Graphics\Mesh\Mesh.h"

void VertexAttribGenerator::Generate(const Mesh & mesh, std::vector<VertexAttribGenerator::VertexAttribution> &vas)
{
	/*
		0 - position
		1 - normal
		2 - uv
		3 - color

		other to be continue...
	*/

	/*
		TODO : only vertex ?
		TODO : performace ?
	*/
	
	const std::vector<Vertex> &meshVertex = mesh.GetVertex();
	
	vas.push_back(VertexAttribution(3, AttribType::FLOAT));
	vas.push_back(VertexAttribution(3, AttribType::FLOAT));
	vas.push_back(VertexAttribution(2, AttribType::FLOAT));
}

/* (int)AttribType::COUNT ??? */
int VertexAttribGenerator::m_attribTypeByteCount[] = { 1, 1, 2, 4 };