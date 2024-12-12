#include "vm/LilithVM.h"
#include "compiler/LilithCompiler.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	LilithCompiler compiler;

	compiler.loadBoolean(false);

	compiler.compile("numbers.llt");

	/*
	compiler.loadConst("Lincoln ");
	compiler.loadConst("Dias");
	compiler.loadInstruction(OP_ADD);

	auto result = llvm.exec(compiler.compile("myname.llt"));
	*/

	auto result = llvm.execFromFile("numbers.llt");

	log(result);

	return 0;
}