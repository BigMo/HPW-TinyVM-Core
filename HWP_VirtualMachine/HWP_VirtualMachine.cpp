// HWP_VirtualMachine.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Instruction.h"
#include "Program.h"
#include "Profiler.h"
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
	SetConsoleTitleA("HWP - VirtualMachine [core]");
	Config::ProcessArgs(argc, argv);
	if (!Config::FileExists)
	{
		std::cout << "> Input-file was not specified or found!" << std::endl;
		system("pause");
		return 0;
	}
	std::cout << "> Executing file:" <<std::endl << "\"" << Config::FilePath << "\"" << std::endl;
	Program prog = Program(Config::FilePath);
	Profiler prof = Profiler(!Config::ProfilingSeparate, &prog);

	Instruction *i = nullptr;
	auto start = std::chrono::high_resolution_clock::now();
	while ((i = prog.FetchNextInstruction()) != nullptr && !prog.HasExited())
	{
		if (Config::PrintInstructions)
			Print(i);

		if(Config::ProfilingEnabled)
			prof.Start();

		prog.ExecuteInstruction(i);

		if (Config::ProfilingEnabled)
		{
			prof.Stop();
			prof.SaveDelta();
		}

		if (Config::SingleStep)
		{
			prog.PrintState();
		}
	}
	auto finish = std::chrono::high_resolution_clock::now();
	double duration = (double)(std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count()) / 1000000000.0;
	std::cout << "> Reached end of program:" << std::endl;
	std::cout << "\tInstructions fetched: " << prog.GetFetchedInstructions() << std::endl;
	std::cout << "\tInstructions executed: " << prog.GetExecutedInstructions() << std::endl;
	std::cout << "\tDuration: " << std::setprecision(8) << duration << "s" << std::endl;
	std::cout << "Performing full dump..." << std::endl;
	prog.Exec_Dmp();

	if (Config::ProfilingEnabled)
	{
		std::cout << "Saving profile..." << std::endl;
		prof.SaveToFile("Profile.txt");
	}

	std::cout << "Done." << std::endl;
	system("pause");
    return 0;
}

