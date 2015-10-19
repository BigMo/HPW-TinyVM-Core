#pragma once
#include "stdafx.h"

//OpCodes
#define NOP		0000
#define LOAD	0001
#define ADD		0010
#define SUB		0011
#define MUL		0100
#define MOV		0101
#define DIV		0110
#define PUSH	0111
#define POP		1000
#define JMP		1001
#define JIZ		1010
#define JIH		1011
#define JSR		1100
#define RTS		1101
#define DMP		1110

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