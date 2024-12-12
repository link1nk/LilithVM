#include "vm/LilithVM.h"
#include "compiler/LilithCompiler.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	LilithCompiler compiler;

	compiler.loadConst(6);
	compiler.loadConst(6);
	
	compiler.loadIf("==");

	compiler.loadConst("Lincoln ");
	compiler.loadConst("Dias");
	compiler.loadInstruction(OP_ADD);

	compiler.loadElse();

	compiler.loadConst(1);
	compiler.loadConst(1);
	compiler.loadInstruction(OP_ADD);

	compiler.commitIf();
	
	compiler.compile("if.llt");

	/*
	auto result = llvm.exec(compiler.compile());
	*/

	auto result = llvm.execFromFile("if.llt");

	log(result);

	return 0;
}