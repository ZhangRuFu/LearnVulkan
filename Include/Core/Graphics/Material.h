#pragma once

class Shader;
class Matrix4x4;

/*
	²ÄÖÊ·â×°
*/

class Material
{
private:
	const Shader& m_shader;

public:
	Material(const Shader &shader);

	const Shader& GetShader(void) const { return m_shader; }

	//void SetMatrix(String& uniformName, Matrix4x4& matrix);
	//void SetMatrixArray(String& uniformName, std::vector<Matrix4x4>& matrixArray);
};
