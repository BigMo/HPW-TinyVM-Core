#pragma once
#include "stdafx.h"

//OpCodes
#define NOP		0
#define LOAD	1
#define ADD		2
#define SUB		3
#define MUL		4
#define MOV		5
#define DIV		6
#define PUSH	7
#define POP		8
#define JMP		9
#define JIZ		10
#define JIH		11
#define JSR		12
#define RTS		13
#define DMP		14
#define BREAK	15

//Struct defining the structure of value.
struct RegisterParameter
{
	word OpCode : 4;
	word RX : 4;		//Index of the destination-register
	word RY : 4;		//Index of the source-register
	word FromMem : 1;	//Read value from mem?
	word ToMem : 1;		//Write value to mem?
	word Unused : 2;	//Padding
};

struct ValueParameter
{
	word OpCode : 4;
	word Value : 12;
};

union Instruction
{
	RegisterParameter RegisterParameter;
	ValueParameter ValueParameter;
};

static void OpCodeToString(word opCode, char* lpcstr)
{
	memset(lpcstr, 0, 5);
	switch (opCode)
	{
	case NOP:
		memcpy(lpcstr, "NOP", 3);
		break;
	case LOAD:
		memcpy(lpcstr, "LOAD", 4);
		break;
	case ADD:
		memcpy(lpcstr, "ADD", 3);
		break;
	case SUB:
		memcpy(lpcstr, "SUB", 3);
		break;
	case MUL:
		memcpy(lpcstr, "MUL", 3);
		break;
	case MOV:
		memcpy(lpcstr, "MOV", 3);
		break;
	case DIV:
		memcpy(lpcstr, "DIV", 3);
		break;
	case PUSH:
		memcpy(lpcstr, "PUSH", 4);
		break;
	case POP:
		memcpy(lpcstr, "POP", 3);
		break;
	case JMP:
		memcpy(lpcstr, "JMP", 3);
		break;
	case JIZ:
		memcpy(lpcstr, "JIZ", 3);
		break;
	case JIH:
		memcpy(lpcstr, "JIH", 3);
		break;
	case JSR:
		memcpy(lpcstr, "JSR", 3);
		break;
	case RTS:
		memcpy(lpcstr, "RTS", 3);
		break;
	case DMP:
		memcpy(lpcstr, "DMP", 3);
		break;
	case BREAK:
		memcpy(lpcstr, "BREAK", 5);
		break;
	}
}

//Struct defining the structure of an instruction
//struct Instruction
//{
//	/* Variables */
//	word Data; //Raw data
//
//	/* Constructors */
//	Instruction() : Data(0)
//	{
//
//	}
//	Instruction(byte OpCode, word value)
//	{
//		Data = OpCode + (value << 4);
//	}
//
//	Instruction(byte OpCode, RegisterParameter parameter)
//	{
//		word data = *(word*)&parameter;
//		Data = OpCode + (data << 4);
//	}
//
//	/* Methods */
//	//Returns the first 4 bytes of Data
//	byte GetOpCode() { return Data & 0x0F; }
//
//	//Returns the last 12 bytes of Data
//	word GetParameter() { return Data >> 4; }
//	
//	void SetOpCode(byte opCode)
//	{
//		Data = opCode & 0x0F + GetParameter();
//	}
//
//	void SetParameter(byte opCode, word data)
//	{
//		Data = opCode & 0x0F + (data << 4);
//	}
//
//	void SetRegisterParameter(byte opCode, word rx, word ry, word fromMem, word toMem)
//	{
//		word data = opCode & 0x0F;
//		data |= rx << 4;
//		data |= ry << 8;
//		data |= fromMem << 12;
//		data |= fromMem << 13;
//	}
//
//	//Casts the parameters of this instruction to RegisterParameters
//	RegisterParameter GetRegisterParameter()
//	{
//		word d = GetParameter();
//		RegisterParameter val = *(RegisterParameter*)&d;
//		return val;
//	}
//};