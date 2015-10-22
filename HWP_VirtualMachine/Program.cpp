#include "stdafx.h"
#include "Program.h"
#include "IOManager.h"

Program::Program(char * fileName) : m_InstructionPointer(0), m_bMemoryAllocated(true), m_bExited(false), m_iExecutedInstructions(0), m_iFetchedInstructions(0), m_pCurrentInstruction(nullptr)
{
	std::ifstream file(fileName, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	if (size == -1)
	{
		this->m_bExited = true;
		return;
	}

	this->m_pProgram = new byte[size];
	this->m_iProgramLength = (dword)size;
	if (!file.read((char*)m_pProgram, size))
		this->m_bExited = true;
}

Program::Program(byte* pProgramBuffer, int iLength) : m_pProgram(pProgramBuffer), m_iProgramLength(iLength), m_InstructionPointer(0), m_bMemoryAllocated(false), m_bExited(false), m_iExecutedInstructions(0), m_iFetchedInstructions(0), m_pCurrentInstruction(nullptr)
{
	for (int i = 0; i < NUM_REGISTERS; i++)
	{
		this->m_pRegisterStacks[i] = Stack<REG_TYPE>();
		this->m_pRegisters[i] = 0;
	}
}

Program::~Program()
{
	if (this->m_bMemoryAllocated)
		delete[] this->m_pProgram;
}

//Fetches the next instruction from code
Instruction * Program::FetchNextInstruction()
{
	if (this->m_InstructionPointer <= this->m_iProgramLength - sizeof(Instruction))
	{
		//Convert instruction from bytes
		this->m_pCurrentInstruction = (Instruction*)(this->m_pProgram + this->m_InstructionPointer);
		//Advance counter
		this->m_LastInstructionPointer = this->m_InstructionPointer;
		this->m_InstructionPointer += sizeof(Instruction);
		this->m_iFetchedInstructions++;
	}
	else
	{
		this->m_bExited = true;
		this->m_pCurrentInstruction = nullptr;
	}
	return this->m_pCurrentInstruction;
}

bool Program::HasExited()
{
	return this->m_bExited;
}

void Program::JumpTo(dword iPos)
{
	m_InstructionPointer = iPos;
}

void Program::ExecuteInstruction(Instruction * pIns)
{
	switch (pIns->RegisterParameter.OpCode)
	{
	case NOP:
		break;
	case LOAD:
		REG(0) = pIns->ValueParameter.Value;
		break;
	case MOV:
		this->Exec_Mov(pIns->RegisterParameter);
		break;
	case ADD:
		this->Exec_Add(pIns->RegisterParameter);
		break;
	case SUB:
		this->Exec_Sub(pIns->RegisterParameter);
		break;
	case MUL:
		this->Exec_Mul(pIns->RegisterParameter);
		break;
	case DIV:
		this->Exec_Div(pIns->RegisterParameter);
		break;
	case PUSH:
		this->Exec_Push(pIns->RegisterParameter);
		break;
	case POP:
		this->Exec_Pop(pIns->RegisterParameter);
		break;
	case JMP:
		this->m_InstructionPointer = pIns->ValueParameter.Value;
		break;
	case JIZ:
		if (REG(0) == 0)
			this->m_InstructionPointer = pIns->ValueParameter.Value;
		break;
	case JIH:
		if (REG(0) > 0)
			this->m_InstructionPointer = pIns->ValueParameter.Value;
		break;
	case JSR:
		this->m_InstructionPointers.Push(this->m_InstructionPointer);
		this->m_InstructionPointer = pIns->ValueParameter.Value;
		break;
	case RTS:
		if (this->m_InstructionPointers.GetFrameCount() > 0)
			this->m_InstructionPointer = this->m_InstructionPointers.Pop();
		else
			this->m_bExited = true;
		break;
	case DMP:
		this->Exec_Dmp();
		break;
	case BREAK:
		this->PrintState();

		break;
	default:
		std::cout << "> Unknown OpCode \"" << pIns->ValueParameter.OpCode << "\"" << std::endl;
		break;
	}
	this->m_iExecutedInstructions++;
}

void Program::Exec_Mov(RegisterParameter param)
{
	REG_CHECK(param.RX);
	REG_CHECK(param.RY);

	if (param.FromMem && param.ToMem) //[rx] = [ry]
	{
		MEM_CHECK(REG(param.RY));
		MEM_CHECK(REG(param.RX));
		this->SetTInMem<REG_TYPE>(REG(param.RX), this->GetTFromMem<REG_TYPE>(REG(param.RY)));
	}
	else if (param.FromMem) //rx = [ry]
	{
		MEM_CHECK(REG(param.RY));
		REG(param.RX) = this->GetTFromMem<REG_TYPE>(REG(param.RY));
	}
	else if (param.ToMem) //[rx] = ry
	{
		MEM_CHECK(REG(param.RX));
		this->SetTInMem<REG_TYPE>(REG(param.RX), REG(param.RY));
	}
	else //rx = ry
	{
		REG(param.RX) = REG(param.RY);
	}
}

void Program::Exec_Add(RegisterParameter param)
{
	//rx = rx + ry
	REG_CHECK(param.RX);
	REG_CHECK(param.RY);

	REG(param.RX) = REG(param.RX) + REG(param.RY);
	CHECK_ZERO(param.RX);
}

void Program::Exec_Sub(RegisterParameter param)
{
	//rx = rx - ry
	REG_CHECK(param.RX);
	REG_CHECK(param.RY);

	REG(param.RX) = REG(param.RX) - REG(param.RY);
	CHECK_ZERO(param.RX);
}

void Program::Exec_Mul(RegisterParameter param)
{
	//rx = rx * ry
	REG_CHECK(param.RX);
	REG_CHECK(param.RY);

	REG(param.RX) = REG(param.RX) * REG(param.RY);
	CHECK_ZERO(param.RX);
}

void Program::Exec_Div(RegisterParameter param)
{
	//rx = rx / ry
	REG_CHECK(param.RX);
	REG_CHECK(param.RY);

	REG(param.RX) = REG(param.RX) / REG(param.RY);
	CHECK_ZERO(param.RX);
}

void Program::Exec_Push(RegisterParameter param)
{
	REG_CHECK(param.RX);
	
	STACK(param.RX).Push(REG(param.RX));
}

void Program::Exec_Pop(RegisterParameter param)
{
	REG_CHECK(param.RX);

	REG(param.RX) = STACK(param.RX).Pop();
}

template <class T>
void WriteRegisterToFile(char* regName, Stack<T> *reg)
{
	char regFileName[32];
	sprintf_s(regFileName, "%s.dmp", regName);
	T *regBuffer = reg->Dump();
	if (regBuffer != nullptr)
		GIOManager.WriteToLocalFile(regFileName, (char*)regBuffer, reg->GetFrameCount() * sizeof(T));
	else
		GIOManager.WriteToLocalFile(regFileName, "empty", 5);
}

void Program::Exec_Dmp()
{
	//Dump memory
	GIOManager.WriteToLocalFile("memory.dmp", (char*)(this->m_pMemory), MEMORY_SIZE);
	//Dump GP registers
	for (int i = 0; i < NUM_REGISTERS; i++)
	{
		char regFileName[8];
		sprintf_s(regFileName, "R%i", i);
		WriteRegisterToFile<REG_TYPE>(regFileName, &(this->m_pRegisterStacks[i]));
	}
	//Dump instruction-pointer-register
	WriteRegisterToFile<dword>("callstack", &(this->m_InstructionPointers));
}

dword Program::GetFetchedInstructions()
{
	return this->m_iFetchedInstructions;
}

dword Program::GetExecutedInstructions()
{
	return this->m_iExecutedInstructions;
}

dword Program::GetInstructionPointer()
{
	return this->m_InstructionPointer;
}

dword Program::GetLastInstructionPointer()
{
	return this->m_LastInstructionPointer;
}

Instruction * Program::GetCurrentInstruction()
{
	return this->m_pCurrentInstruction;
}

//Prints the state of the program
void Program::PrintState()
{
	std::cout << "--------------------------------------------------------" << std::endl;
	//Fetched/executed instructions, IP
	std::cout << " State: " << this->m_iFetchedInstructions << "F " << this->m_iExecutedInstructions << "E, @0x" << std::setw(4) << std::setfill('0') << std::hex << this->m_InstructionPointer << std::endl;
	//Registers
	std::cout.width(0);
	for (int i = 0; i < NUM_REGISTERS; i++) {
		std::cout << std::setw(4) << std::setfill(' ') << std::dec << i;
		if (i < NUM_REGISTERS - 1)
			std::cout << "|";
	}
	std::cout << std::endl;
	for (int i = 0; i < NUM_REGISTERS; i++) {
		std::cout << std::setw(4) << std::setfill(' ') << std::hex << this->m_pRegisters[i];
		if (i < NUM_REGISTERS - 1)
			std::cout << "|";
	}
	std::cout << std::endl;
	//Callstack
	std::cout << "Callstack: ";
	PrintStack();

	//std::string input;
	//std::cout << "(Press enter to continue execution)" << std::endl;
	//std::cin >> input;
	system("pause");
	std::cout << "--------------------------------------------------------" << std::endl;
}

void Program::PrintStack()
{
	Stack<dword> tmpStack = Stack<dword>();

	while (this->m_InstructionPointers.GetFrameCount() > 0)
		tmpStack.Push(this->m_InstructionPointers.Pop());

	while (tmpStack.GetFrameCount() > 0)
	{
		dword ptr = tmpStack.Pop();
		this->m_InstructionPointers.Push(ptr);
		std::cout << std::setw(2) << std::setfill(' ') << std::hex << ptr << "->";
	}

	std::cout << std::setw(2) << std::setfill(' ') << std::hex << this->m_InstructionPointer << std::dec << std::endl;
}
