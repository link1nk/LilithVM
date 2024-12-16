#include "LilithCompiler.h"

std::map<std::string, uint8_t> LilithCompiler::compareOps
{ {"<", 0}, {">", 1}, {"==", 2}, {">=", 3}, {"<=", 4}, {"!=", 5} };

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

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4333)
#endif
void LilithCompiler::patchJumpAddress(size_t offset, uint32_t value)
{
    writeByteAtOffset(offset, (value >> 24) & 0xff);
    writeByteAtOffset(offset + 1, (value >> 16) & 0xff);
    writeByteAtOffset(offset + 2, (value >> 8) & 0xff);
    writeByteAtOffset(offset + 3, value & 0xff);
}
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

void LilithCompiler::writeByteAtOffset(size_t offset, uint8_t value)
{
    std::cout << "writing: " << offset << " value -> " << (int)value << '\n';
    co->code[offset] = value;
}

bool LilithCompiler::isGlobalScope()
{
    return co->scopeLevel == 1 && co->name == "main";
}

size_t LilithCompiler::getVarsCountOnScopeExit()
{
    auto varsCount = 0;

    if (co->locals.size() > 0)
    {
        while (!co->locals.empty() && co->locals.back().scopeLevel == co->scopeLevel)
        {
            co->locals.pop_back();
            varsCount++;
        }
    }

    return varsCount;
}

LilithCompiler::LilithCompiler(std::shared_ptr<Global> global) :
    global(global), disassembler(std::make_unique<LilithDisassembler>(global))
{
    LilithValue llv = ALLOC_CODE("main");

    co = AS_CODE(llv);

    startBlock();
}

CodeObject* LilithCompiler::compile()
{
    endBlock();

    emit(OP_HALT);

    return co;
}

CodeObject* LilithCompiler::compile(std::string file)
{
    emit(OP_HALT);

    std::vector<CodeObject*> codes = { co };

    endBlock();

    LilithFileWriter::writeToFile(file, codes, *global);

    return co;
}

void LilithCompiler::setGlobal(std::shared_ptr<Global> global)
{
    this->global = global;
    disassembler->setGlobal(global);
}

void LilithCompiler::disassembleBytecode(CodeObject* co)
{
    disassembler->disassemble(co);
}

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable: 4267)
#endif
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
    if (!op.empty())
    {
        loadCompare(op);
    }
    emit(OP_JMP_IF_FALSE);
    emit(0);
    emit(0);
    emit(0);
    emit(0);

    IfElseBlock block;
    block.elseJmpAddr = getOffset() - 4;
    ifElseStack.push(block);
}

void LilithCompiler::startElse()
{
    emit(OP_JMP);
    emit(0);
    emit(0);
    emit(0);
    emit(0);

    if (ifElseStack.empty()) {
        throw std::runtime_error("Mismatched 'else': no corresponding 'if' block.");
    }

    auto& block = ifElseStack.top();
    block.endAddr = getOffset() - 4;
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
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

void LilithCompiler::accessVariable(const std::string& name)
{
    auto localIndex = co->getLocalIndex(name);

    if (localIndex != -1)
    {
        emit(OP_GET_LOCAL);
        emit(localIndex);
    }
    else
    {
        if (!global->exists(name))
        {
            DIE << "[LilithCompiler]: Reference Error: " << name;
        }

        emit(OP_GET_GLOBAL);
        emit(global->getGlobalIndex(name));
    }
}

void LilithCompiler::setVariable(const std::string& name, double value)
{
    loadConst(value);

    if (isGlobalScope())
    {
        global->define(name);

        emit(OP_SET_GLOBAL);
        emit(global->getGlobalIndex(name));
    }
    else
    {
        co->addLocal(name);
        emit(OP_SET_LOCAL);
        emit(co->getLocalIndex(name));
    }
}

void LilithCompiler::setVariable(const std::string& name, std::string value)
{
    loadConst(value);

    if (isGlobalScope())
    {

        global->define(name);

        emit(OP_SET_GLOBAL);
        emit(global->getGlobalIndex(name));
    }
    else
    {

        co->addLocal(name);
        emit(OP_SET_LOCAL);
        emit(co->getLocalIndex(name));
    }
}

void LilithCompiler::updateVariable(const std::string& name)
{
    auto localIndex = co->getLocalIndex(name);

    if (localIndex != -1)
    {
        emit(OP_SET_LOCAL);
        emit(localIndex);
    }
    else
    {
        auto globalIndex = global->getGlobalIndex(name);

        if (globalIndex == -1)
        {
            DIE << "Reference error: " << name << " is not defined!";
        }

        emit(OP_SET_GLOBAL);
        emit(globalIndex);
    }
}

void LilithCompiler::updateVariable(const std::string& name, double value)
{
    loadConst(value);

    auto localIndex = co->getLocalIndex(name);

    if (localIndex != -1)
    {
        emit(OP_SET_LOCAL);
        emit(localIndex);
    }
    else
    {
        auto globalIndex = global->getGlobalIndex(name);

        if (globalIndex == -1)
        {
            DIE << "Reference error: " << name << " is not defined!";
        }

        emit(OP_SET_GLOBAL);
        emit(globalIndex);
    }
}

void LilithCompiler::updateVariable(const std::string& name, std::string value)
{
    loadConst(value);

    auto localIndex = co->getLocalIndex(name);
    
    if (localIndex != -1)
    {
        emit(OP_SET_LOCAL);
        emit(localIndex);
    }
    else
    {
        auto globalIndex = global->getGlobalIndex(name);

        if (globalIndex == -1)
        {
            DIE << "Reference error: " << name << " is not defined!";
        }

        emit(OP_SET_GLOBAL);
        emit(globalIndex);
    }
}

void LilithCompiler::startBlock()
{
    co->scopeLevel++;
}

void LilithCompiler::endBlock()
{
    auto varsCount = getVarsCountOnScopeExit();

    if (varsCount > 0)
    {
        emit(OP_SCOPE_EXIT);
        emit(varsCount);
    }

    co->scopeLevel--;
}

void LilithCompiler::initWhile()
{
    WhileBlock block;
    block.loopStartAddr = getOffset();

    whileStack.push(block);
}

void LilithCompiler::setWhileCondition(const std::string& op)
{
    if (!op.empty())
    {
        loadCompare(op);
    }

    emit(OP_JMP_IF_FALSE);
    emit(0);
    emit(0);
    emit(0);
    emit(0);

    auto& block = whileStack.top();
    block.loopEndJmpAddr = getOffset() - 4;
}

void LilithCompiler::endWhile()
{
    emit(OP_JMP);
    emit(0);
    emit(0);
    emit(0);
    emit(0);

    auto& block = whileStack.top();

    size_t jumpToStartOffset = getOffset(); 
    patchJumpAddress(getOffset() - 4, block.loopStartAddr); 

    std::cout << "here\n";

    block.loopEndAddr = getOffset();
    patchJumpAddress(block.loopEndJmpAddr, block.loopEndAddr); 

    whileStack.pop(); 
}

void LilithCompiler::loadInstruction(uint8_t opcode)
{
    emit(opcode);
}