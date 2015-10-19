// HWP_VirtualMachine.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Program.h"

void Print(Instruction* ins)
{
	std::cout << "RegisterParameter:" << std::endl;
	std::cout << "\tOpCode: " << ins->RegisterParameter.OpCode << std::endl;
	std::cout << "\tRX: " << ins->RegisterParameter.RX << std::endl;
	std::cout << "\tRY: " << ins->RegisterParameter.RY << std::endl;
	std::cout << "\tToMem: " << ins->RegisterParameter.ToMem << std::endl;
	std::cout << "\tFromMem: " << ins->RegisterParameter.FromMem << std::endl;
	std::cout << "\tUnused: " << ins->RegisterParameter.Unused << std::endl;
	
	std::cout << "ValueParameter:" << std::endl;
	std::cout << "\tOpCode: " << ins->ValueParameter.OpCode << std::endl;
	std::cout << "\tValue: " << ins->ValueParameter.Value << std::endl;
}

int main()
{
	char path[512];
	GetCurrentDirectoryA(512, path);
	strcat_s(path, "\\testprog1.bin");

	Program prog = Program(path);
	
	Instruction *i = prog.FetchNextInstruction();
	if (i == nullptr)
	{
		std::cout << "Could not fetch instruction!" << std::endl;
	}
	else 
	{
		do
		{
			Print(i);
			i = prog.FetchNextInstruction();
		} while (i != nullptr && !prog.HasExited());
		prog.Exec_Dmp();
	}
	system("pause");
    return 0;
}

