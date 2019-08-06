#pragma once
#include <vector>
/*
	转换Mesh容器到顶点属性数据流,并生成对应描述标识
*/

class Mesh;

class VertexAttribGenerator
{
private:
	static int m_attribTypeByteCount[];

public:
	enum class AttribType { UNSIGHED_BYTE, BYTE, HALF_FLOAT, FLOAT, COUNT };

	class VertexAttribution
	{
	public:
		const AttribType m_attribType;	//type
		const int m_componentSize;		//component size
		const int m_size;				//size in byte

		VertexAttribution(int componentSize, AttribType attribType) : m_attribType(attribType), m_componentSize(componentSize), m_size(componentSize * m_attribTypeByteCount[(int)m_attribType]) {}
	};

private:
	VertexAttribGenerator(void) {}

public:
	static void Generate(const Mesh& mesh, std::vector<VertexAttribution> &vas);

};