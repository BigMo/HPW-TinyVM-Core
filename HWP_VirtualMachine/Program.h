#pragma once
#include "Instruction.h"
#include "Stack.h"

#define NUM_REGISTERS 16
#define MEMORY_SIZE 4096
#define REG_TYPE int

#define REG_VALID(idx) (idx < NUM_REGISTERS)
#define MEM_VALID(address) (address < MEMORY_SIZE)
#define REG_CHECK(idx) if(!REG_VALID(idx)) { printf("> ERROR [@%X]: Invalid register %i\n", this->m_InstructionPointer, idx); return; }
#define MEM_CHECK(address) if(!MEM_VALID(address)) { printf("> ERROR [@%X]: Invalid memory-location %i\n", this->m_InstructionPointer, address); return; }
#define REG(idx) this->m_pRegisters[idx]
#define STACK(idx) this->m_pRegisterStacks[idx]
#define CHECK_ZERO(idx) { REG(0) = REG(idx); }

class Program
{
public:
	Program(char* fileName);
	Program(byte* pProgramBuffer, int iLength);
	~Program();
	Instruction* FetchNextInstruction();
	void ExecuteInstruction(Instruction* pIns);
	bool HasExited();
	void Exec_Dmp();
private:
	/* Variables */
	byte* m_pProgram;
	byte m_pMemory[MEMORY_SIZE];
	dword m_InstructionPointer;
	Stack<REG_TYPE> m_pRegisterStacks[NUM_REGISTERS];
	Stack<dword> m_InstructionPointers;
	REG_TYPE m_pRegisters[NUM_REGISTERS];
	bool m_bExited;
	bool m_bMemoryAllocated;
	dword m_iProgramLength;

	/* Methods */
	void JumpTo(dword iPos);
	template<class T>
	T GetTFromMem(dword index)
	{
		if (MEM_VALID(index))
			return *(T*)((char*)this->m_pMemory + index);
		return T();
	}
	template<class T>
	void SetTInMem(dword index, T value)
	{
		if (MEM_VALID(index))
			*(T*)((char*)m_pMemory + index) = value;
	}

	/* Arithmetic */
	void Exec_Add(RegisterParameter param);
	void Exec_Sub(RegisterParameter param);
	void Exec_Mul(RegisterParameter param);
	void Exec_Div(RegisterParameter param);

	/* Data-storage */
	void Exec_Mov(RegisterParameter param);
	void Exec_Push(RegisterParameter param);
	void Exec_Pop(RegisterParameter param);
};

