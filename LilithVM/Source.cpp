#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.compiler->updateGlobalVariable("x", 20.0);
	
	llvm.compiler->compile("execute.llt");

	auto result = llvm.execFromFile("execute.llt");

	std::cout << std::endl;

	log(result);

	return 0;
}