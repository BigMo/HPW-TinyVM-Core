#pragma once
#include "stdafx.h"

#define MAX_PATH_LENGTH 512

class IOManager
{
public:
	char* BuildFilePath(char* localFile);
	std::ofstream* OpenLocalFileBinary(char* localFile);
	void WriteToLocalFile(char* localFile, char* buffer, int length);
};

extern IOManager GIOManager;