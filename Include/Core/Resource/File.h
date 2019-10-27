#pragma once
#include "Core\Container\String.h"
#include "Core\Log\Debug.h"

/*
	File Tool
*/

class Resource
{
public:
	static int ReadTextFile(String fileName, String& fileContent);
};