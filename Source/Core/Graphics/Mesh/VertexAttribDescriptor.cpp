#include "Core\Graphics\Mesh\VertexAttribDescriptor.h"
#include "Core\Graphics\Mesh\Mesh.h"

VertexAttribDescriptor::VertexAttribDescriptor(const Mesh & mesh)
{
	GenerateVertexAttribDescription(mesh);
}

void VertexAttribDescriptor::GenerateVertexAttribDescription(const Mesh & mesh)
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
	
	int stride = 2 * (3 * sizeof(float)) + 2 * sizeof(float);
	m_vertexAttribUnit.push_back(Unit(0, 0, stride, AttribType::FLOAT));
	m_vertexAttribUnit.push_back(Unit(1, 3 * sizeof(float), stride, AttribType::FLOAT));
	m_vertexAttribUnit.push_back(Unit(2, 2 * 3 * sizeof(float), stride, AttribType::FLOAT));

	char* buffer = new char[stride * meshVertex.size()];
	char* bufferPointer = buffer;
	std::vector<Vertex>::const_iterator i = meshVertex.begin();
	
	while (i != meshVertex.end())
	{
		memcpy(bufferPointer, &(i->m_position), 3 * sizeof(float));
		bufferPointer += 3 * sizeof(float);
		memcpy(bufferPointer, &(i->m_normal), 3 * sizeof(float));
		bufferPointer += 3 * sizeof(float);
		memcpy(bufferPointer, &(i->m_texCoord), 2 * sizeof(float));
		bufferPointer += 2 * sizeof(float);
	}


}
