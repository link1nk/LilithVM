#include "LilithCompiler.h"

std::map<std::string, uint8_t> LilithCompiler::compareOps
{ {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5} };

std::unique_ptr<LilithDisassembler> LilithCompiler::disassembler
{ std::make_unique<LilithDisassembler>() };

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

size_t LilithCompiler::booleanConstIdx(bool value)
{
    ALLOC_CONST(IS_BOOLEAN, AS_BOOLEAN, BOOLEAN, value);
    return co->constants.size() - 1;
}

size_t LilithCompiler::getOffset()
{
    return co->code.size();
}

void LilithCompiler::patchJumpAddress(size_t offset, uint16_t value)
{
    writeByteAtOffset(offset, (value >> 8) & 0xff);
    writeByteAtOffset(offset + 1, value & 0xff);
}

void LilithCompiler::writeByteAtOffset(size_t offset, uint8_t value)
{
    co->code[offset] = value;
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

void LilithCompiler::disassembleBytecode(CodeObject* co)
{
    disassembler->disassemble(co);
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

void LilithCompiler::loadBoolean(bool boolean)
{
    emit(OP_CONST);
    emit(booleanConstIdx(boolean));
}

void LilithCompiler::loadCompare(std::string op)
{
    emit(OP_COMPARE);
    emit(compareOps[op]);
}

void LilithCompiler::startIf(std::string op)
{
    if (!op.empty()) {
        loadCompare(op);
    }
    emit(OP_JMP_IF_FALSE);
    emit(0xaa);
    emit(0xbb);

    IfElseBlock block;
    block.elseJmpAddr = getOffset() - 2;
    ifElseStack.push(block);
}

void LilithCompiler::startElse()
{
    emit(OP_JMP);
    emit(0xcc);
    emit(0xdd);

    if (ifElseStack.empty()) {
        throw std::runtime_error("Mismatched 'else': no corresponding 'if' block.");
    }

    auto& block = ifElseStack.top();
    block.endAddr = getOffset() - 2;
    block.elseBranchAddr = getOffset();

    patchJumpAddress(block.elseJmpAddr, block.elseBranchAddr);
}

void LilithCompiler::endIf()
{
    if (ifElseStack.empty()) {
        throw std::runtime_error("Mismatched 'endIf': no corresponding 'if' block.");
    }

    auto block = ifElseStack.top();
    ifElseStack.pop();

    block.endBranchAddr = getOffset();
    patchJumpAddress(block.endAddr, block.endBranchAddr);
}

void LilithCompiler::loadInstruction(uint8_t opcode)
{
    emit(opcode);
}