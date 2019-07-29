#include "Core\Graphics\Mesh\Mesh.h"
#include "Core\Math\MathTrick.h"

Mesh::Mesh(String meshPath) 
{
	//load mesh by path
	//need share ???
}

Mesh::Mesh(MeshType meshType) : m_vertices(m_basicMesh[(int)meshType].GetVertex()), m_indices(m_basicMesh[(int)meshType].GetIndex())
{
	//No share
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : m_vertices(vertices), m_indices(indices) {}

int Mesh::GetVertexCount() const
{
	return m_vertices.size();
}

int Mesh::GetIndexCount() const
{
	return m_indices.size();
}

const std::vector<Vertex>& Mesh::GetVertex() const
{
	return m_vertices;
}

const std::vector<unsigned int>& Mesh::GetIndex() const
{
	return m_indices;
}

const Mesh* Mesh::InitBasicMesh(void)
{
	//Init Basic Mesh
	Mesh *basicMeshes = new Mesh[(int)Mesh::MeshType::MeshTypeCount];

	float cubeVertices[] = {
		// Positions          // Normals           // Texture Coords
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
	};
	int rectIndex[] = { 0, 1, 2, 2, 3, 0 };
	int cubeVertexGroupCount = sizeof(cubeVertices) / (8 * 4);
	std::vector<Vertex> cubeVertex(cubeVertexGroupCount);
	for (int i = 0; i < cubeVertexGroupCount; i++)
	{
		Vertex vertex;
		for (int j = 0; j < 3; j++)
			vertex.m_position[j] = cubeVertices[i * 8 + j];
		for (int j = 0; j < 3; j++)
			vertex.m_normal[j] = cubeVertices[i * 8 + 3 + j];
		for (int j = 0; j < 2; j++)
			vertex.m_texCoord[j] = cubeVertices[i * 8 + 6 + j];
		cubeVertex[i] = (vertex);
	}
	std::vector<unsigned int> cubeIndex;
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
			cubeIndex.push_back(i * 4 + rectIndex[j]);

	basicMeshes[(int)Mesh::MeshType::Cube].SetVertex(cubeVertex);
	basicMeshes[(int)Mesh::MeshType::Cube].SetIndex(cubeIndex);

	float planeVertices[] = {
		// Positions			//Normal			// Texture Coords
		0.5f,  0.0f, 0.5f,		0.0f, 1.0f,	0.0f,	5.0f, 0.0f,
		-0.5f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 5.0f,
		0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	5.0f, 5.0f,
	};
	int planeVertexGroupCount = sizeof(planeVertices) / (8 * 4);
	std::vector<Vertex> planeVertex;
	for (int i = 0; i < planeVertexGroupCount; i++)
	{
		Vertex vertex;
		for (int j = 0; j < 3; j++)
			vertex.m_position[j] = planeVertices[i * 8 + j];
		for (int j = 0; j < 3; j++)
			vertex.m_normal[j] = planeVertices[i * 8 + 3 + j];
		for (int j = 0; j < 2; j++)
			vertex.m_texCoord[j] = planeVertices[i * 8 + 6 + j];
		planeVertex[i] = (vertex);
	}
	std::vector<unsigned int> planeIndex;
	for (int i = 0; i < 6; i++)
		planeIndex.push_back(cubeIndex[i]);
	
	basicMeshes[(int)Mesh::MeshType::Plane].SetVertex(planeVertex);
	basicMeshes[(int)Mesh::MeshType::Plane].SetIndex(planeIndex);

	//Sphere
	float thetaStep = 10;
	float betaStep = 18;
	int betaCount = 360 / betaStep;
	int thetaCount = 180 / thetaStep + 1;
	int sphereVertexCount = thetaCount * betaCount;
	thetaStep = Deg2Rad(thetaStep);
	betaStep = Deg2Rad(betaStep);
	std::vector<Vertex> sphereVertex;

	int sIndex = 0;
	float theta = 0;
	for (int i = 0; i < thetaCount; ++i)
	{
		float y = cos(theta);
		float r = sin(theta);
		float beta = 0;
		for (int j = 0; j < betaCount; ++j)
		{
			sphereVertex[sIndex].m_position.X = r * cos(beta);
			sphereVertex[sIndex].m_position.Y = y;
			sphereVertex[sIndex].m_position.Z = r * sin(beta);
			sphereVertex[sIndex].m_normal = sphereVertex[sIndex].m_position;
			sphereVertex[sIndex].m_texCoord.X = beta / (Deg2Rad(360.0));
			sphereVertex[sIndex].m_texCoord.Y = 1 - theta / Deg2Rad(180.0);
			sIndex++;
			beta += betaStep;
		}
		theta += thetaStep;
	}

	std::vector<unsigned int> sphereIndex;

	for (int i = 0; i < betaCount - 1; i++)
	{
		sphereIndex.push_back(i);
		sphereIndex.push_back(betaCount + i);
		sphereIndex.push_back(betaCount + i + 1);
	}
	sphereIndex.push_back(betaCount - 1);
	sphereIndex.push_back(2 * betaCount - 1);
	sphereIndex.push_back(2 * betaCount);

	int curIndex = 0;
	for (int i = 1; i < thetaCount - 2; i++)
	{
		for (int j = 0; j < betaCount - 1; j++)
		{
			curIndex = i * betaCount + j;
			sphereIndex.push_back(curIndex);
			sphereIndex.push_back(curIndex + betaCount);
			sphereIndex.push_back(curIndex + betaCount + 1);
			sphereIndex.push_back(curIndex + betaCount + 1);
			sphereIndex.push_back(curIndex + 1);
			sphereIndex.push_back(curIndex);
		}
		curIndex = (i + 1) * betaCount - 1;
		sphereIndex.push_back(curIndex);
		sphereIndex.push_back(curIndex + betaCount);
		sphereIndex.push_back(curIndex + 1);
		sphereIndex.push_back(curIndex + 1);
		sphereIndex.push_back(curIndex - betaCount + 1);
		sphereIndex.push_back(curIndex);
	}

	curIndex = (thetaCount - 1) * betaCount;
	for (int i = 0; i < betaCount - 1; i++)
	{
		sphereIndex.push_back(curIndex + i);
		sphereIndex.push_back(curIndex + i - betaCount);
		sphereIndex.push_back(curIndex + i - betaCount + 1);
	}
	sphereIndex.push_back(curIndex + betaCount - 1);
	sphereIndex.push_back(curIndex - 1);
	sphereIndex.push_back(curIndex - betaCount);

	basicMeshes[(int)Mesh::MeshType::Sphere].SetVertex(sphereVertex);
	basicMeshes[(int)Mesh::MeshType::Sphere].SetIndex(sphereIndex);

	return basicMeshes;
}

const Mesh* Mesh::m_basicMesh = Mesh::InitBasicMesh();