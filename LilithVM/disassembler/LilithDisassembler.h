/*
* Lilith disassembler
*/

#ifndef LilithDisassembler_H
#define LilithDisassembler_H

#include <array>
#include "../vm/LilithValue.h"

class LilithDisassembler
{
private:
	static std::array<std::string, 6> inverseCompareOps;

	size_t disassembleInstruction(CodeObject* co, size_t offset);
	size_t disassembleSimple(CodeObject* co, uint8_t opcode, size_t offset);
	void dumpBytes(CodeObject* co, size_t offset, size_t count);
	void printOpcode(uint8_t opcode);
	size_t disassembleConst(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleCompare(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleJump(CodeObject* co, uint8_t opcode, size_t offset);
	uint16_t readWordOffset(CodeObject* co, size_t offset);

public:
	void disassemble(CodeObject* co);                               // Disassembles a code unit
};

#endif // LilithDisassembler_H