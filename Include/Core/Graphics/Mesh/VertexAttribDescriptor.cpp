#include "VertexAttribDescriptor.h"

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


}
