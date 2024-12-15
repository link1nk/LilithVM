#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.compiler->setVariable("i", 10);
	llvm.compiler->setVariable("count", 0);

	llvm.compiler->initWhile();
	llvm.compiler->accessVariable("i");
	llvm.compiler->accessVariable("count");
	llvm.compiler->setWhileCondition(">");

	llvm.compiler->startBlock();
	
	llvm.compiler->accessVariable("i");
	llvm.compiler->loadConst(1);
	llvm.compiler->loadInstruction(OP_SUB);
	llvm.compiler->updateVariable("i");

	llvm.compiler->accessVariable("count");
	llvm.compiler->loadConst(1);
	llvm.compiler->loadInstruction(OP_ADD);
	llvm.compiler->updateVariable("count");

	llvm.compiler->endBlock();

	llvm.compiler->endWhile();

	llvm.compiler->accessVariable("count");

	//llvm.compiler->compile("execute.llt");

	auto result = llvm.exec(llvm.compiler->compile());

	std::cout << std::endl;

	log(result);

	return 0;
}