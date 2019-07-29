#include "Core\Misc\File.h"

int File::ReadTextFile(String fileName, String& fileContent)
{
	FILE *fp = nullptr;
	errno_t result = fopen_s(&fp, fileName.c_str(), "r");
	if (result)
	{
		Debug::Error(StringUtil::format("Can not read file {0}", fileName));
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	int len = ftell(fp);
	rewind(fp);
	char* source = new char[len + 1]();
	fread(source, len, 1, fp);
	len = strlen(source);
	fclose(fp);

	/*
		TODO : ???
		TODO : performance
	*/
	fileContent = source;
	delete[] source;

	return len;
}