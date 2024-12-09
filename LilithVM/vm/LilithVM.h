/*
*  Lilith Virtual Machine
*/

#ifndef LilithVM_H
#define LilithVM_H

#include <string>
#include <vector>
#include <cstddef>
#include <array>

#include "../bytecode/OpCode.h"
#include "Logger.h"
#include "LilithValue.h"

/*
* Reads the current byte in the bytecode
* and advances the ip pointer
*/
#define READ_BYTE() static_cast<int>(*ip++)

/*
* Gets a constant from the pool
*/
#define GET_CONST() constants[READ_BYTE()]

/*
* Stack top (stack overflow after exceeding)
*/
#define STACK_LIMIT 512

/*
* Binary operations
*/
#define BINARY_OP(op) \
	{ \
		auto op2 = AS_NUMBER(pop()); \
		auto op1 = AS_NUMBER(pop()); \
		push(NUMBER(op1 op op2)); \
	}

class LilithVM
{
private:
	std::vector<uint8_t> code;                     // Bytecode
	std::vector<LilithValue> constants;            // Constant pool
	std::array<LilithValue, STACK_LIMIT> stack;    // Operands stack
	uint8_t* ip;                                   // Instruction pointer
	LilithValue* sp;                               // Stack pointer

	LilithValue eval();                            // Main eval loop
	void push(const LilithValue& value);           // Pushes a value onto the stack
	LilithValue pop();                             // Pops a value from the stack
public:
	LilithVM();

	LilithValue exec(const std::string& program);  // Executes a program

};

#endif // LilithVM_H