#pragma once
#include <vector>

#include "Core\Math\Vector3.h"
#include "Core\Container\String.h"

/*
 *	Basic Vertex
 *	TODO : 考虑顶点属性的拓展性
*/
struct Vertex
{
	Vector3 m_position;
	Vector3 m_normal;
	Vector3 m_texCoord;
};

/*
 *	Static Mesh[internal in engine]
 *	No Animation(Vertex/Skeleton)
 *	No Texture
 */
class Mesh
{
public:
	enum class MeshType { Cube, Plane, Sphere, MeshTypeCount };

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	//attrib description


public:
	Mesh(void) {};
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	Mesh(String meshPath);
	Mesh(MeshType meshType);

	int GetVertexCount() const;
	int GetIndexCount() const;

	const std::vector<Vertex>& GetVertex() const;
	const std::vector<unsigned int>& GetIndex() const;

	void SetVertex(std::vector<Vertex> &vertices) { m_vertices = vertices; }
	void SetIndex(std::vector<unsigned int> &indices) { m_indices = indices; }

private:
	static const Mesh* m_basicMesh;
	static const Mesh* InitBasicMesh(void);
};