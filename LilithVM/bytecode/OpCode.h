/*
* Instruction set for Lilith VM
*/

#ifndef OpCode_H
#define OpCode_H

#include <cstddef>
#include "../vm/Logger.h"

constexpr uint8_t OP_HALT         { 0x00 };  // Stops the program
constexpr uint8_t OP_CONST        { 0x01 };  // Pushes a const onto the stack
constexpr uint8_t OP_ADD          { 0x02 };  // Add instruction
constexpr uint8_t OP_SUB          { 0x03 };  // Sub instruction
constexpr uint8_t OP_DIV          { 0x04 };  // Div instruction
constexpr uint8_t OP_MUL          { 0x05 };  // Mul instruction
constexpr uint8_t OP_COMPARE      { 0x06 };  // Comparison
constexpr uint8_t OP_JMP_IF_FALSE { 0x07 };  // Jump if the value on the stack is false
constexpr uint8_t OP_JMP          { 0x08 };  // Unconditional jump

#define OP_STR(op) \
	case OP_##op:  \
		return #op 

static std::string opcodeToString(uint8_t opcode)
{
	switch (opcode)
	{
		OP_STR(HALT);
		OP_STR(CONST);
		OP_STR(ADD);
		OP_STR(SUB);
		OP_STR(DIV);
		OP_STR(MUL);
		OP_STR(COMPARE);
		OP_STR(JMP_IF_FALSE);
		OP_STR(JMP);
	default:
		DIE << "opcodeToString: unknown opcode: " << (int)opcode;
	}
	return "Unknown"; // Unreachable
}

#endif // OpCode_H