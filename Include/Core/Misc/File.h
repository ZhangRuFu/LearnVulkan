#pragma once
#include "Core\Container\String.h"
#include "Core\Log\Debug.h"

/*
	File Tool
*/

class File
{
public:
	static int ReadTextFile(String fileName, String& fileContent);
};