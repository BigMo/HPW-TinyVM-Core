// HWP_VirtualMachine.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Program.h"
#include "Config.h"
#pragma comment( lib, "shlwapi.lib")

void Print(Instruction* ins)
{
	char opCode[5];
	OpCodeToString(ins->RegisterParameter.OpCode, opCode);
	std::cout << "OpCode: " << opCode << " (" << ins->RegisterParameter.OpCode << ")"<< std::endl;

	std::cout << "RegisterParameter:" << std::endl;
	std::cout << "\tRX: " << ins->RegisterParameter.RX << std::endl;
	std::cout << "\tRY: " << ins->RegisterParameter.RY << std::endl;
	std::cout << "\tToMem: " << ins->RegisterParameter.ToMem << std::endl;
	std::cout << "\tFromMem: " << ins->RegisterParameter.FromMem << std::endl;
	std::cout << "\tUnused: " << ins->RegisterParameter.Unused << std::endl;
	
	std::cout << "ValueParameter:" << std::endl;
	std::cout << "\tValue: " << ins->ValueParameter.Value << std::endl;
}

int main(int argc, char** argv)
{
	Config::ProcessArgs(argc, argv);
	if (!Config::FileExists)
	{
		std::cout << "> Input-file was not specified or found!" << std::endl;
		system("pause");
		return 0;
	}
	std::cout << "> Executing file:" <<std::endl << "\"" << Config::FilePath << "\"" << std::endl;

	Program prog = Program(Config::FilePath);
	
	Instruction *i = nullptr;
	while ((i = prog.FetchNextInstruction()) != nullptr && !prog.HasExited())
	{
		//if (Config::PrintInstructions)
		//	Print(i);
		prog.ExecuteInstruction(i);
		/*if (Config::SingleStep)
		{
			prog.PrintState();
		}*/
	}
	prog.Exec_Dmp();
	std::cout << "> Reached end of program:" << std::endl;
	std::cout << "\tInstructions fetched: " << prog.GetFetchedInstructions() << std::endl;
	std::cout << "\tInstructions executed: " << prog.GetExecutedInstructions() << std::endl;
	system("pause");
    return 0;
}

