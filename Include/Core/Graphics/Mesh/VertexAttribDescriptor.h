#pragma once
#include <vector>
/*
	转换Mesh容器到顶点属性数据流,并生成对应描述标识
*/

class Mesh;

class VertexAttribDescriptor
{
public:
	enum class AttribType { UNSIGHED_BYTE, BYTE, HALF_FLOAT, FLOAT };

	class Unit
	{
	public:
		const unsigned int m_index;	//attrib index
		const int m_offset;			//start offset
		const int m_stride;			//stride
		const AttribType m_attribType;//type

		Unit(unsigned int index, int offset, int stride, AttribType attribType) : m_index(index), m_offset(offset), m_stride(stride), m_attribType(attribType) {}
	};

private:
	std::vector<Unit> m_vertexAttribUnit;

public:
	VertexAttribDescriptor(const Mesh& mesh);

private:
	void GenerateVertexAttribDescription(const Mesh& mesh);

};