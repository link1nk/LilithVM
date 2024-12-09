#include "LilithVM.h"

LilithVM::LilithVM() :
	ip(nullptr), sp(nullptr), stack({})
{
}

LilithValue LilithVM::exec(const std::string& program)
{
	LilithValue llv[] = { ALLOC_STRING("Lincoln "), ALLOC_STRING("Dias") };

	for (auto e : llv)
	{
		constants.push_back(e);
	}

	code = { OP_CONST, 0, OP_CONST, 1, OP_ADD, OP_HALT };

	// Init the stack
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
		//---------------------------------------------------
		// Exit program
		//---------------------------------------------------
		case OP_HALT:
		{
			return pop();
		}
		//---------------------------------------------------


		//---------------------------------------------------
		// Handle constants
		//---------------------------------------------------
		case OP_CONST:
		{
			push(GET_CONST());
			break;
		}
		//---------------------------------------------------


		//---------------------------------------------------
		// Math operations
		//---------------------------------------------------
		case OP_ADD:
		{
			auto op2 = pop();
			auto op1 = pop();

			if (IS_NUMBER(op1) && IS_NUMBER(op2))
			{
				auto v1 = AS_NUMBER(op1);
				auto v2 = AS_NUMBER(op2);
				push(NUMBER(v1 + v2));
			}
			else if (IS_STRING(op1) && IS_STRING(op2))
			{
				auto v1 = AS_CPPSTRING(op1);
				auto v2 = AS_CPPSTRING(op2);
				push(ALLOC_STRING(v1 + v2));
			}

			break;
		}

		case OP_SUB:
		{
			BINARY_OP(-);
			break;
		}

		case OP_DIV:
		{
			BINARY_OP(/ );
			break;
		}

		case OP_MUL:
		{
			BINARY_OP(*);
			break;
		}
		//---------------------------------------------------


		default:
		{
			DIE << "Unknow opcode: " << std::hex << opcode;
		}
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