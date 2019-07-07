#pragma once
#include "Core\Container\String.h"

/*
 *	µ÷ÊÔ»úÖÆ
*/
class Debug
{
public:
	static void Log(const String& logStr);
	static void Warning(const String& warStr);
	static void Error(const String& errStr);
};