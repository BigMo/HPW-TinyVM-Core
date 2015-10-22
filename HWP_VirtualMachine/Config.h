#pragma once
#include "stdafx.h"

namespace Config
{
	bool SingleStep = false;
	bool PrintInstructions = false;
	bool ProfilingEnabled = false;
	bool ProfilingSeparate = false;

	char FilePath[512];
	bool FileExists = false;

	//Processes the command-line parameters passed
	void ProcessArgs(int argc, char** argv)
	{
		if (argc == 1)
		{
			std::cout << "* Usage:" << std::endl;
			std::cout << "* core.exe <program-file> [-singleStep] [-printInstructions] [-profilingEnabled] [-profilingSeparate]:" << std::endl;
			std::cout << "* \tsingleStep: Steps through every single instruction" << std::endl;
			std::cout << "* \tprintInstructions: Prints every fetched instruction (opcode and parameters)" << std::endl;
			std::cout << "* \tprofilingEnabled: Enables profiling (disabled by default)" << std::endl;
			std::cout << "* \tprofilingSeparate: Sets profiling-mode to separate (sum by default)" << std::endl;
			return;
		}

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
			if (strcmp("-singleStep", argv[i]) == 0)
				SingleStep = true;
			if (strcmp("-printInstructions", argv[i]) == 0)
			PrintInstructions = true;
			if (strcmp("-profilingEnabled", argv[i]) == 0)
				ProfilingEnabled = true;
			if (strcmp("-profilingSeparate", argv[i]) == 0)
				ProfilingSeparate = true;
		}
	}
}