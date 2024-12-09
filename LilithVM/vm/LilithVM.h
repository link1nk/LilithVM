/*
*  Lilith Virtual Machine
*/

#ifndef LilithVM_H
#define LilithVM_H

#include <string>
#include <vector>
#include <cstddef>

#include "../bytecode/OpCode.h"
#include "Logger.h"

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

#endif // __LilithVM_H