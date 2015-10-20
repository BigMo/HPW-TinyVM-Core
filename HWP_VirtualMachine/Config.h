#pragma once
#include "stdafx.h"

namespace Config
{
	bool SingleStep = false;
	bool PrintInstructions = false;

	char FilePath[512];
	bool FileExists = false;

	//Processes the command-line parameters passed
	void ProcessArgs(int argc, char** argv)
	{
		if (argc == 1)
			return;

		strcpy_s(FilePath, argv[1]);
		if (!PathFileExistsA(FilePath)) //In case the specified file doesn't exist, search inside the current directory
		{
			char path[512];
			GetCurrentDirectoryA(512, path);
			strcat_s(path, "\\");
			strcat_s(path, FilePath);
			strcpy_s(FilePath, path);
			FileExists = PathFileExistsA(FilePath);
		}
		else
		{
			FileExists = true;
		}

		if (!FileExists)
			return;

		for (int i = 1; i < argc; i++)
		{
			SingleStep = strcmp("-singleStep", argv[i]) >= 0;
			PrintInstructions = strcmp("-printInstructions", argv[i]) >= 0;
		}
	}
}