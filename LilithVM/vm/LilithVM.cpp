#include "LilithVM.h"

LilithVM::LilithVM() :
	ip(nullptr), sp(nullptr)
{
}

LilithValue LilithVM::exec(const std::string& program)
{
	LilithValue llv = NUMBER(10);

	constants.push_back(llv);

	code = { OP_CONST, 0, OP_HALT };

	sp = &stack[0];

	// Set instruction pointer to the beginning
	ip = &code[0];

	return eval();
}

LilithValue LilithVM::eval()
{
	for (;;)
	{
		auto opcode{ READ_BYTE() };

#ifdef _DEBUG
		log(opcode);
#endif // _DEBUG

		switch (opcode)
		{
			// Exit program
		case OP_HALT:
			return pop();

			// Handle constants
		case OP_CONST:
			push(GET_CONST());
			break;

		default:
			DIE << "Unknow opcode: " << std::hex << opcode;
		}
	}
}

void LilithVM::push(const LilithValue& value)
{
	if ((size_t)(sp - stack.data()) == STACK_LIMIT)
	{
		DIE << "push(): Stack Overflow\n";
	}
	*sp = value;
	sp++;
}

LilithValue LilithVM::pop()
{
	if (sp == stack.data())
	{
		DIE << "pop(): empty stack\n";
	}
	--sp;
	return *sp;
}