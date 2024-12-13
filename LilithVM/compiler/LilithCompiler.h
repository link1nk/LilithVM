#ifndef Compiler_H
#define Compiler_H

#include <string>
#include <map>
#include <stack>
#include "../vm/LilithValue.h"
#include "../bytecode/OpCode.h"
#include "../bytecode/LilithFile.h"
#include "../disassembler/LilithDisassembler.h"

#define ALLOC_CONST(tester, converter, allocator, value)    \
	{                                                       \
		for (auto i = 0; i < co->constants.size(); i++)     \
		{                                                   \
			if (!tester(co->constants[i]))                  \
			{                                               \
				continue;                                   \
			}                                               \
			if (converter(co->constants[i]) == value)       \
			{                                               \
				return i;                                   \
			}                                               \
		}                                                   \
		co->constants.push_back(allocator(value));          \
	}                                                     
 
class LilithCompiler
{
private:
	CodeObject* co;                                           // Compiling code object
	static std::map<std::string, uint8_t> compareOps;      
	static std::unique_ptr<LilithDisassembler> disassembler;  // Disassembler

	struct IfElseBlock
	{
		size_t elseJmpAddr;
		size_t endAddr;
		size_t elseBranchAddr;
		size_t endBranchAddr;
	};

	std::stack<IfElseBlock> ifElseStack;
					       
	void emit(uint8_t code);                               // Emits data to the bytecode
	size_t numericConstIdx(double value);                  // Allocates a numeric constant
	size_t numericConstIdx(std::string value);             // Allocates a string constant
	size_t booleanConstIdx(bool value);                    // Allocates a boolean constant
	size_t getOffset();                                    // Returns current bytecode offset
	void patchJumpAddress(size_t offset, uint16_t value);  // Patches jump address
	void writeByteAtOffset(size_t offset, uint8_t value);  // Writes byte at offset

public:
	LilithCompiler();

	CodeObject* compile();
	CodeObject* compile(std::string file);

	static void disassembleBytecode(CodeObject* co);

	void loadConst(double constant);
	void loadConst(std::string constant);
	void loadBoolean(bool boolean);
	void loadCompare(std::string op);
	void startIf(std::string op);
	void startElse();
	void endIf();
	void loadInstruction(uint8_t opcode);
};

#endif // Compiler_H