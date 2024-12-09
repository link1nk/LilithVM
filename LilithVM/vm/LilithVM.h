/*
*  Lilith Virtual Machine
*/

#ifndef LILITH_VM
#define LILITH_VM

#include <string>
#include <vector>
#include <cstddef>

#include "OpCode.h"

#define READ_BYTE() *ip++

class LilithVM
{
private:
	std::vector<std::byte> code;  // Bytecode
	std::byte* ip;                // Instruction pointer

public:
	LilithVM();

	void exec(const std::string& program); /* Executes a program */
	void eval();                           /* Main eval loop */
};

#endif // __LILITH_VM