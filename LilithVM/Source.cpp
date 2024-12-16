#include "vm/LilithVM.h"
#include "bytecode/LilithFile.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.compiler->nativeFunctionSquare("VERSION");

	CodeObject* co = llvm.compiler->compile("execute.llt");
	auto result = llvm.execFromFile("execute.llt");

	//auto result = llvm.exec(co);

	std::cout << std::endl;

	log(result);


	return 0;
}