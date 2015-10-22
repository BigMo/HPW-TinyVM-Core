#include "stdafx.h"
#include "IOManager.h"

IOManager GIOManager;

char* IOManager::BuildFilePath(char* localFile)
{
	char* path = new char[MAX_PATH_LENGTH];
	GetCurrentDirectoryA(MAX_PATH_LENGTH, path);
	strcat_s(path, MAX_PATH_LENGTH, "\\");
	strcat_s(path, MAX_PATH_LENGTH, localFile);
	return path;
}

std::ofstream* IOManager::OpenLocalFileBinary(char* localFile)
{
	char* path = BuildFilePath(localFile);
	std::ofstream *file = new std::ofstream(path, std::ios::binary);
	delete[] path;
	return file;
}

void IOManager::WriteToLocalFile(char* localFile, char* buffer, int length)
{
	std::ofstream *file = /*this->*/OpenLocalFileBinary(localFile);
	file->write(buffer, length);
	file->close();
	delete file;
}