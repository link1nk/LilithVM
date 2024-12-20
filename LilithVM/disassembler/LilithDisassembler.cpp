#include "LilithDisassembler.h"
#include "../bytecode/OpCode.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

std::array<std::string, 6> LilithDisassembler::inverseCompareOps
{ "<", ">", "==", ">=", "<=", "!=" };

size_t LilithDisassembler::disassembleInstruction(CodeObject* co, size_t offset)
{
	std::ios_base::fmtflags f{ std::cout.flags() };

	std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(8)
		      << offset << "    ";

	auto opcode = co->code[offset];

	switch (opcode)
	{
	case OP_POP:
	case OP_ADD:
	case OP_SUB:
	case OP_DIV:
	case OP_MUL:
	case OP_HALT:
		return disassembleSimple(co, opcode, offset);
	case OP_CONST:
		return disassembleConst(co, opcode, offset);
	case OP_COMPARE:
		return disassembleCompare(co, opcode, offset);
	case OP_JMP_IF_FALSE:
	case OP_JMP:
		return disassembleJump(co, opcode, offset);
	case OP_GET_GLOBAL:
	case OP_SET_GLOBAL:
		return disassembleGlobal(co, opcode, offset);
	case OP_GET_LOCAL:
	case OP_SET_LOCAL:
		return disassembleLocal(co, opcode, offset);
	case OP_SCOPE_EXIT:
	case OP_CALL:
		return disassembleWord(co, opcode, offset);
	default:
		DIE << "disassemblyInstruction: no disassembly for "
			<< opcodeToString(opcode);
	}

	std::cout.flags(f);

	return 0; // Unreachable
}

size_t LilithDisassembler::disassembleSimple(CodeObject* co, uint8_t opcode, size_t offset)
{
	dumpBytes(co, offset, 1);
	printOpcode(opcode);
	return offset + 1;
}

void LilithDisassembler::dumpBytes(CodeObject* co, size_t offset, size_t count)
{
	std::ios_base::fmtflags f{ std::cout.flags() };

	std::stringstream ss;

	for (size_t i = 0; i < count; i++)
	{
		ss << std::uppercase << std::hex << std::setfill('0') << std::setw(2)
			<< (((int)co->code[offset + i]) & 0xFF) << " ";
	}

	std::cout << std::left << std::setfill(' ') << std::setw(20) << ss.str();
	std::cout.flags(f);
}

void LilithDisassembler::printOpcode(uint8_t opcode)
{
	std::ios_base::fmtflags f{ std::cout.flags() };

	std::cout << std::left << std::setfill(' ') << std::setw(20)
		      << opcodeToString(opcode) << " ";

	std::cout.flags(f);
}

size_t LilithDisassembler::disassembleConst(CodeObject* co, uint8_t opcode, size_t offset)
{
	dumpBytes(co, offset, 2);
	printOpcode(opcode);
	
	auto constIndex = co->code[offset + 1];

	std::cout << (int)constIndex << " ("
		<< lilithValueToConstantString(co->constants[constIndex]) << ")";
	
	return offset + 2;
}

size_t LilithDisassembler::disassembleCompare(CodeObject* co, uint8_t opcode, size_t offset)
{
	dumpBytes(co, offset, 2);
	printOpcode(opcode);

	auto compareOp = co->code[offset + 1];

	std::cout << (int)compareOp << " (";
	std::cout << inverseCompareOps[compareOp] << ")";
	return offset + 2;
}

size_t LilithDisassembler::disassembleJump(CodeObject* co, uint8_t opcode, size_t offset)
{
	std::ios_base::fmtflags f{ std::cout.flags() };

	dumpBytes(co, offset, 5);
	printOpcode(opcode);
	uint32_t address = readDwordOffset(co, offset + 1);

	std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(8)
		<< (int)address << " ";

	std::cout.flags(f);

	return offset + 5;
}

size_t LilithDisassembler::disassembleGlobal(CodeObject* co, uint8_t opcode, size_t offset)
{
	dumpBytes(co, offset, 2);
	printOpcode(opcode);

	auto globalIndex = co->code[offset + 1];

	std::cout << (int)globalIndex << " (" << global->get(globalIndex).name << ")";

	return offset + 2;
}

size_t LilithDisassembler::disassembleLocal(CodeObject* co, uint8_t opcode, size_t offset)
{
	dumpBytes(co, offset, 2);
	printOpcode(opcode);

	auto localIndex = co->code[offset + 1];

	/*
	std::string localName = co->locals[localIndex].name;

	if (localName.rfind("DELETED", 0) == 0)
	{
		std::cout << (int)localIndex << " (" << localName.substr(7) << ")";

		co->locals.erase(co->locals.begin() + localIndex);
	}
	else
	{
		std::cout << (int)localIndex << " (" << localName << ")";
	}
	*/

	std::cout << (int)localIndex;

	return offset + 2;
}

size_t LilithDisassembler::disassembleWord(CodeObject* co, uint8_t opcode, size_t offset)
{
	dumpBytes(co, offset, 2);
	printOpcode(opcode);
	std::cout << (int)co->code[offset + 1];
	return offset + 2;
}

uint32_t LilithDisassembler::readDwordOffset(CodeObject* co, size_t offset)
{
	return (uint32_t)((co->code[offset] << 24) |
		(co->code[offset + 1] << 16) |
		(co->code[offset + 2] << 8) |
		co->code[offset + 3]);
}

LilithDisassembler::LilithDisassembler(std::shared_ptr<Global> global) :
	global(global)
{}

void LilithDisassembler::disassemble(CodeObject* co)
{
	std::cout << "\n------------------------ Disassembly: " << co->name
			  << " ------------------------\n\n";

	size_t offset{ 0 };

	while (offset < co->code.size())
	{
		offset = disassembleInstruction(co, offset);
		std::cout << "\n";
	}
}

void LilithDisassembler::setGlobal(std::shared_ptr<Global> global)
{
	this->global = global;
}
