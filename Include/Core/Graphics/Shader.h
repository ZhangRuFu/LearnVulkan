#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "Core\Container\String.h"

/*
	Shader 封装
	TODO : Shader 资源管理
*/

class Shader
{
private:
	String m_shaderPath;
	String m_vertexShaderSource;
	String m_fragmentShaderSource;

public:
	/*
		.vs -> vertex shader
		.fs -> fragment shader
		不需要指定后缀，确保都在统一目录下
		TODO : ShaderLab
	*/
	Shader(String shaderPath);

public:
	String GetShaderPath(void) { return m_shaderPath; }
	String GetVertexShaderSource(void) { return m_vertexShaderSource; };
	String GetFragmentShaderSource(void) { return m_fragmentShaderSource; };
};