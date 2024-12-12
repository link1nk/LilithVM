#include "LilithCompiler.h"

void LilithCompiler::emit(uint8_t code)
{
    co->code.push_back(code);
}

size_t LilithCompiler::numericConstIdx(double value)
{
	ALLOC_CONST(IS_NUMBER, AS_NUMBER, NUMBER, value);
	return co->constants.size() - 1;
}

size_t LilithCompiler::numericConstIdx(std::string value)
{
	ALLOC_CONST(IS_STRING, AS_CPPSTRING, ALLOC_STRING, value);
	return co->constants.size() - 1;
}

LilithCompiler::LilithCompiler()
{
    LilithValue llv = ALLOC_CODE("main");

    co = AS_CODE(llv);
}

CodeObject* LilithCompiler::compile()
{
    emit(OP_HALT);

    std::vector<CodeObject*> codes = { co };

    return co;
}

CodeObject* LilithCompiler::compile(std::string file)
{
    emit(OP_HALT);

    std::vector<CodeObject*> codes = { co };

    LilithFileWriter::writeToFile(file, codes);

    return co;
}

void LilithCompiler::loadConst(double constant)
{
    emit(OP_CONST);
    emit(numericConstIdx(constant));
}

void LilithCompiler::loadConst(std::string constant)
{
    emit(OP_CONST);
    emit(numericConstIdx(constant));
}

void LilithCompiler::loadInstruction(uint8_t opcode)
{
    emit(opcode);
}