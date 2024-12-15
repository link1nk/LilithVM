/*
* Lilith disassembler
*/

#ifndef LilithDisassembler_H
#define LilithDisassembler_H

#include <array>
#include <memory>
#include "../vm/LilithValue.h"
#include "../vm/Global.h"

class LilithDisassembler
{
private:
	static std::array<std::string, 6> inverseCompareOps;
	std::shared_ptr<Global> global;

	size_t disassembleInstruction(CodeObject* co, size_t offset);
	size_t disassembleSimple(CodeObject* co, uint8_t opcode, size_t offset);
	void dumpBytes(CodeObject* co, size_t offset, size_t count);
	void printOpcode(uint8_t opcode);
	size_t disassembleConst(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleCompare(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleJump(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleGlobal(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleLocal(CodeObject* co, uint8_t opcode, size_t offset);
	size_t disassembleWord(CodeObject* co, uint8_t opcode, size_t offset);
	uint32_t readDwordOffset(CodeObject* co, size_t offset);

public:
	LilithDisassembler(std::shared_ptr<Global> global);

	void disassemble(CodeObject* co);                               // Disassembles a code unit
	void setGlobal(std::shared_ptr<Global> global);
};

#endif // LilithDisassembler_H