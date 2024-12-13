#include "Global.h"

void Global::set(size_t index, const LilithValue& value)
{
	if (index >= globals.size())
	{
		DIE << "Global " << index << " doesn't exist";
	}
	globals[index].value = value;
}

int Global::getGlobalIndex(const std::string& name)
{
	if (globals.size() < 0)
	{
		return -1;
	}

	for (auto i = (int) globals.size() - 1; i >= 0; i--)
	{
		if (globals[i].name == name)
		{
			return i;
		}
	}

	return -1;
}

void Global::define(const std::string& name)
{
	auto index = getGlobalIndex(name);

	// Already defined
	if (index != -1)
	{
		return;
	}

	// Set to default number 0
	globals.push_back({ name, NUMBER(0) });
}

void Global::addConst(const std::string& name, double value)
{
	if (exists(name))
	{
		return;
	}

	globals.push_back({ name, NUMBER(value) });
}
