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
#include "../bytecode/LilithFile.h"
#include "../compiler/LilithCompiler.h"
#include "Logger.h"
#include "LilithValue.h"
#include "Global.h"

/*
* Reads the current byte in the bytecode
* and advances the ip pointer
*/
#define READ_BYTE() static_cast<int>(*ip++)

/*
* Reads a dword (4 bytes)
*/
#define READ_DWORD() (ip += 4, \
                   (uint32_t)((ip[-4] << 24) | (ip[-3] << 16) | (ip[-2] << 8) | ip[-1]))

/*
* Converts bytecode index to a pointer
*/
#define TO_ADDRESS(index) (&co->code[index])

/*
* Gets a constant from the pool
*/
#define GET_CONST() (co->constants[READ_BYTE()])

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

/*
* Generic values comparison
*/
#define COMPARE_VALUES(op, v1, v2) \
		bool res = false;          \
		switch (op)                \
		{                          \
		case 0:                    \
			res = v1 < v2;         \
			break;                 \
		case 1:                    \
			res = v1 > v2;         \
			break;                 \
		case 2:                    \
			res = v1 == v2;        \
			break;                 \
		case 3:                    \
			res = v1 >= v2;        \
			break;                 \
		case 4:                    \
			res = v1 <= v2;        \
			break;                 \
		case 5:                    \
			res = v1 != v2;        \
			break;                 \
		}                          \
		push(BOOLEAN(res));

class LilithVM
{
private:
	CodeObject* co;                                // Code object
	std::array<LilithValue, STACK_LIMIT> stack;    // Operands stack
	uint8_t* ip;                                   // Instruction pointer
	LilithValue* sp;                               // Stack 

	std::shared_ptr<Global> global;                // Global object

	LilithValue eval();                            // Main eval loop
	void push(const LilithValue& value);           // Pushes a value onto the stack
	LilithValue pop();                             // Pops a value from the stack
	LilithValue peek(size_t offset = 0);           // Peeks an element from the stack
	void setGlobalVariables();                     // Sets up global variables and functions

public:
	std::unique_ptr<LilithCompiler> compiler;      // Compiler

	LilithVM();

	LilithValue exec(CodeObject* program);         // Executes a program
	LilithValue execFromFile(std::string program); // Executes a program from lilith bytecode file
};

#endif // LilithVM_H