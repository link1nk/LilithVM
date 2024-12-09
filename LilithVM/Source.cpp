#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	llvm.exec(R"(
		42	
	)");

	return 0;
}