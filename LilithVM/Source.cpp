#include "vm/LilithVM.h"
#include <iostream>

int main(void)
{
	LilithVM llvm;

	auto result = llvm.exec(R"(
		42	
	)");

	log(AS_NUMBER(result));

	return 0;
}