#include "LilithVM.h"

LilithVM::LilithVM() :
	ip(nullptr)
{}

void LilithVM::exec(const std::string& program)
{
	code = { std::byte(1) };

	ip = &code[0];

	return eval();
}

void LilithVM::eval()
{
	for (;;)
	{
		auto opcode{ std::to_integer<int>(READ_BYTE()) };

#ifdef _DEBUG
		log(opcode);
#endif // _DEBUG

		switch (opcode)
		{
		case std::to_integer<int>(OP_HALT):
			return;

		default:
			DIE << "Unknow opcode: " << std::hex << opcode;
		}
	}
}