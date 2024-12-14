#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.compiler->loadConst(6);
	llvm.compiler->loadConst(6);
	
	llvm.compiler->startIf("!=");

	llvm.compiler->setGlobalVariable("lilith1", 20);
	llvm.compiler->accessGlobalVariable("x");
	llvm.compiler->loadInstruction(OP_ADD);

	llvm.compiler->startElse();

	llvm.compiler->setGlobalVariable("lilith2", 10);
	llvm.compiler->accessGlobalVariable("y");
	llvm.compiler->loadInstruction(OP_ADD);

	llvm.compiler->endIf();
	
	llvm.compiler->compile("execute.llt");

	auto result = llvm.execFromFile("execute.llt");

	std::cout << std::endl;

	log(result);

	return 0;
}