#include "vm/LilithVM.h"
#include "compiler/LilithCompiler.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	LilithCompiler compiler;

	compiler.loadConst(6);
	compiler.loadConst(6);
	compiler.compare("==");
	
	compiler.compile("comparison.llt");

	/*
	auto result = llvm.exec(compiler.compile());
	*/

	auto result = llvm.execFromFile("comparison.llt");

	log(result);

	return 0;
}