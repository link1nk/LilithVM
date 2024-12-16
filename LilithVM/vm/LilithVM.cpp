#include "LilithVM.h"
#include "../compiler/LilithCompiler.h"
#include <string>

LilithVM::LilithVM() :
	co(nullptr), ip(nullptr), sp(nullptr), bp(nullptr), stack({}), global(std::make_shared<Global>()), compiler(std::make_unique<LilithCompiler>(global))
{
	setGlobalVariables();
}

LilithValue LilithVM::exec(CodeObject* program)
{
	co = program;

	// Init the stack
	sp = &stack[0];

	// Init the base pointer
	bp = sp;

	// Set instruction pointer to the beginning
	ip = &co->code[0];

#ifdef _DEBUG
	// Debug disassembly:
	compiler->disassembleBytecode(co);
#endif // _DEBUG

	return eval();
}

LilithValue LilithVM::execFromFile(std::string file)
{
	// Loads the CodeObject and global variables
	auto [codeObjects, loadedGlobal] = LilithFileReader::readFromFile(file);

	// Assigns the first CodeObject as the entry point
	co = codeObjects[0];

	// Sets global variables
	global = std::make_shared<Global>(std::move(loadedGlobal));

	// Initialize the stack
	sp = &stack[0];

	// Init the base pointer
	bp = sp;

	// Sets the instruction pointer to the beginning of the code
	ip = &co->code[0];

	compiler->setGlobal(global);

#ifdef _DEBUG
	// Debug do bytecode
	compiler->disassembleBytecode(co);
#endif // _DEBUG

	// Executa o código
	return eval();
}

LilithValue LilithVM::eval()
{
	for (;;)
	{
		auto opcode{ READ_BYTE() };

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
			BINARY_OP(/);
			break;
		}

		case OP_MUL:
		{
			BINARY_OP(*);
			break;
		}
		//---------------------------------------------------

		//---------------------------------------------------
		// Comparison operation
		//---------------------------------------------------
		case OP_COMPARE:
		{
			auto op = READ_BYTE();

			auto op2 = pop();
			auto op1 = pop();

			if (IS_NUMBER(op1) && IS_NUMBER(op2))
			{
				auto v1 = AS_NUMBER(op1);
				auto v2 = AS_NUMBER(op2);
				COMPARE_VALUES(op, v1, v2);
			}
			else if (IS_STRING(op1) && IS_STRING(op2))
			{
				auto s1 = AS_CPPSTRING(op1);
				auto s2 = AS_CPPSTRING(op2);
				COMPARE_VALUES(op, s1, s2);
			}
			break;
		}

		//---------------------------------------------------
		// Conditional jump
		//---------------------------------------------------
		case OP_JMP_IF_FALSE:
		{
			auto cond = AS_BOOLEAN(pop());

			auto address = READ_DWORD();

			if (!cond)
			{
				ip = TO_ADDRESS(address);
			}

			break;
		}
		
		case OP_JMP:
			ip = TO_ADDRESS(READ_DWORD());
			break;

		//---------------------------------------------------
		// Global variable value
		//---------------------------------------------------
		case OP_GET_GLOBAL:
		{
			auto globalIndex = READ_BYTE();
			push(global->get(globalIndex).value);
			break;
		}

		case OP_SET_GLOBAL:
		{
			auto globalIndex = READ_BYTE();
			auto value = peek(0);
			global->set(globalIndex, value);
			break;
		}

		//---------------------------------------------------
		// Stack manipulation
		//---------------------------------------------------
		case OP_POP:
			pop();
			break;

		case OP_GET_LOCAL:
		{
			auto localIndex = READ_BYTE();
			if (localIndex < 0 || localIndex >= stack.size())
			{
				DIE << "OP_GET_LOCAL: Invalid variable index: " << (int)localIndex;
			}
			push(bp[localIndex]);
			break;
		}
			

		case OP_SET_LOCAL:
		{
			auto localIndex = READ_BYTE();
			if (localIndex < 0 || localIndex >= stack.size())
			{
				DIE << "OP_GET_LOCAL: Invalid variable index: " << (int)localIndex;
			}
			auto value = peek(0);
			bp[localIndex] = value;
			break;
		}

		case OP_SCOPE_EXIT:
		{
			auto count = READ_BYTE();

			*(sp - 1 - count) = peek(0);

			popN(count);

			break;
		}

		default:
		{
			DIE << "Unknow opcode: " << std::hex << opcode;
		}
		}

#ifdef _DEBUG
		dumpStack();
#endif // _DEBUG
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

void LilithVM::popN(size_t count)
{
	if (stack.size() == 0)
	{
		DIE << "popN(): empty stack";
	}

	sp -= count;
}

LilithValue LilithVM::peek(size_t offset)
{
	if (stack.size() == 0)
	{
		DIE << "peek(): empty stack\n";
	}
	return *(sp - 1 - offset);
}

void LilithVM::setGlobalVariables()
{
	global->addConst("VERSION", 1);
}

void LilithVM::dumpStack()
{
	std::cout << "\n------------------------- Stack -------------------------\n";

	if (sp == &stack[0])
	{
		std::cout << "(empty)\n";
	}
	auto csp = sp - 1;
	while (csp >= &stack[0])
	{
		std::cout << "Address: " << (void*) csp << " -> " << *csp-- << "\n";
	}
	std::cout << "\n";

	std::cout << "Stack Pointer -> " << (void*)(sp-1) << '\n';
	std::cout << "Base  Pointer -> " << (void*)(bp) << '\n';
}
