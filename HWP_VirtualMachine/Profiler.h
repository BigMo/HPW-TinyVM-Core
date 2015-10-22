#pragma once
#include <chrono>
#include "DynArray.h"
#include "Program.h"
#include "LogInstruction.h"
#include "Instruction.h"

class Profiler
{
public:
	Profiler(bool sum, Program* prog);
	~Profiler();
	void Start();
	void Stop();
	void SaveDelta();
	void SaveToFile(char* filePath);
private:
	Program *m_pProg;
	bool m_bSum;
	DynArray<LogInstruction> m_DynArray = DynArray<LogInstruction>(1.5f);
	dword m_dwCurrentIndex;
	std::chrono::time_point<std::chrono::steady_clock> m_tpStart;
	std::chrono::time_point<std::chrono::steady_clock> m_tpStop;
};

