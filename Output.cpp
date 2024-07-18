#include "Output.h"
#include <iostream>

// See https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

CColourScope::CColourScope(const char* code)
{
	std::cout << "\x1B[" << code << "m";
}

CColourScope::~CColourScope()
{
	std::cout << "\033[0m";
}
