#include "LilithVM.h"

LilithVM::LilithVM() :
	ip(nullptr)
{}

void LilithVM::exec(const std::string& program)
{
	code = { OP_HALT };

	ip = &code[0];

	return eval();
}

void LilithVM::eval()
{
	for (;;)
	{
		switch (std::to_integer<int>(READ_BYTE()))
		{
		case std::to_integer<int>(OP_HALT): return;
		}
	}
}