#include "Core\Graphics\Shader.h"
#include "Core\Misc\File.h"

Shader::Shader(String shaderPath) : m_shaderPath(shaderPath)
{
	static char* shaderName[2]{ ".vs", ".fs" };
	
	String fullShaderPath = shaderPath + ".vs";
	
	int len = File::ReadTextFile(fullShaderPath, m_vertexShaderSource);
	if (len == -1)
	{
		Debug::Error("Can not create vertex shader");
		return;
	}
	
	fullShaderPath.replace(fullShaderPath.find_last_of("vs") - 1, 2, "fs");

	len = File::ReadTextFile(fullShaderPath, m_fragmentShaderSource);
	if (len == -1)
	{
		Debug::Error("Can not create fragment shader");
		return;
	}
}