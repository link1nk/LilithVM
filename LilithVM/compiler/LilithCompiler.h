#ifndef Compiler_H
#define Compiler_H

#include <string>
#include "../vm/LilithValue.h"
#include "../bytecode/OpCode.h"
#include "../bytecode/LilithFile.h"

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
	CodeObject* co;                             // Compiling code object

	void emit(uint8_t code);                    // Emits data to the bytecode
	size_t numericConstIdx(double value);       // Allocates a numeric constant
	size_t numericConstIdx(std::string value);  // Allocates a string constant

public:
	LilithCompiler();

	CodeObject* compile();
	CodeObject* compile(std::string file);
	void loadConst(double constant);
	void loadConst(std::string constant);
	void loadInstruction(uint8_t opcode);
};

#endif // Compiler_H