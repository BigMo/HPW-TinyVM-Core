#pragma once
#include "stdafx.h"

struct LogInstruction
{
	byte OpCode = 0;
	dword Address = 0;
	double Duration = 0;
	dword Count = 0;
};