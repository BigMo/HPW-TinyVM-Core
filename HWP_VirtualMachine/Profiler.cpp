#include "stdafx.h"
#include "Profiler.h"
#include "IOManager.h"

Profiler::Profiler(bool sum, Program* prog) : m_bSum(sum), m_dwCurrentIndex(0), m_pProg(prog)
{
	this->m_tpStart = std::chrono::time_point<std::chrono::steady_clock>();
	this->m_tpStop = std::chrono::time_point<std::chrono::steady_clock>();
}


Profiler::~Profiler()
{
}

void Profiler::Start()
{
	this->m_tpStart = std::chrono::high_resolution_clock::now();
}

void Profiler::Stop()
{
	this->m_tpStop = std::chrono::high_resolution_clock::now();
}

void Profiler::SaveDelta()
{
	qword delta = std::chrono::duration_cast<std::chrono::nanoseconds>(this->m_tpStop - this->m_tpStart).count();
	
	//Save to array
	dword address = this->m_pProg->GetLastInstructionPointer();
	dword idx = this->m_bSum ? address / sizeof(Instruction) : this->m_dwCurrentIndex;

	this->m_DynArray[idx].Address = address;
	this->m_DynArray[idx].Count++;
	this->m_DynArray[idx].OpCode = (byte)this->m_pProg->GetCurrentInstruction()->RegisterParameter.OpCode;
	if (this->m_bSum)
	{
		this->m_DynArray[idx].Duration += delta / 1000000000.0;
		if (idx + 1 > this->m_dwCurrentIndex)
			this->m_dwCurrentIndex = idx + 1;
	}
	else
	{
		this->m_DynArray[this->m_dwCurrentIndex].Duration = (double)delta;
		this->m_dwCurrentIndex++;
	}
}

void Profiler::SaveToFile(char * filePath)
{
	std::ofstream *file = GIOManager.OpenLocalFileBinary(filePath);
	
	(*file) << "Address\tHit-count\tOpCode\tDuration (" << (this->m_bSum ? "s" : "ns") << ")" << std::endl;
	char opCode[5];
	for (dword i = 0; i < this->m_dwCurrentIndex; i++)
	{
		OpCodeToString(this->m_DynArray[i].OpCode, opCode);
		(*file) << "0x" << std::setfill('0') << std::hex << std::setw(4) << this->m_DynArray[i].Address << "\t" <<
			"[" << std::dec << std::setw(7) << this->m_DynArray[i].Count << "]\t" <<
			std::setw(5) << std::setfill(' ') << opCode << "\t" <<
			std::dec << std::setw(0) << (this->m_bSum ? std::setprecision(9) : std::setprecision(0)) << std::fixed << this->m_DynArray[i].Duration << (this->m_bSum ? "s" : "ns") << std::endl;
	}

	file->close();
	delete file;
}
