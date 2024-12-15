#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.compiler->setVariable("x", 5);

	llvm.compiler->startBlock();
	llvm.compiler->setVariable("x", 10);
	llvm.compiler->endBlock();

	llvm.compiler->accessVariable("x");
	
	//llvm.compiler->compile("execute.llt");

	auto result = llvm.exec(llvm.compiler->compile());

	std::cout << std::endl;

	log(result);

	return 0;
}