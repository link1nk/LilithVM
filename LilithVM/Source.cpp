#include "vm/LilithVM.h"
#include "bytecode/LilithFile.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.compiler->setVariable("x", 10);
	llvm.compiler->nativeFunctionSquare("x");

	CodeObject* co = llvm.compiler->compile("execute.llt");
	auto result = llvm.exec("execute.llt");

	//auto result = llvm.exec(co);

	std::cout << std::endl;

	log(result);


	return 0;
}