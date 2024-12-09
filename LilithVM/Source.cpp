#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	auto result = llvm.exec(R"(
		42	
	)");

	log(result);

	return 0;
}