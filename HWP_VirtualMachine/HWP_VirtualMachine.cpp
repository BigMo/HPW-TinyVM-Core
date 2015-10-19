// HWP_VirtualMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Instruction.h"
#include "Program.h"

#define SETBITS(data, mask, shiftl) data = (word)(dt | (mask << shiftl));

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
	/*word dt = 0;
	SETBITS(dt, 1, 0);
	SETBITS(dt, 2, 4);
	SETBITS(dt, 3, 8);
	SETBITS(dt, 0, 12);
	SETBITS(dt, 1, 13);
	Instruction *ins = (Instruction*)&dt;
	Print(ins);
	
	dt = 0;
	SETBITS(dt, 2, 0);
	SETBITS(dt, 4095, 4);
	Print(ins);*/

	Program prog = Program("C:\\Users\\M\\Documents\\Visual Studio 2015\\Projects\\HWP_VirtualMachine\\Debug\\testprog1.bin");
	
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

