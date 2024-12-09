#include "LilithValue.h"

// String representation used in constants for debug
std::string lilithValueToTypeString(const LilithValue& lilithValue)
{
	if (IS_NUMBER(lilithValue))
	{
		return "NUMBER";
	}
	else if (IS_STRING(lilithValue))
	{
		return "STRING";
	}
	else if (IS_CODE(lilithValue))
	{
		return "CODE";
	}
	else
	{
		DIE << "lilithValueToTypeString: unknow type " << (int)lilithValue.type;
	}
	return ""; // Unreachable
}

// String representation used in constants for debug
std::string lilithValueToConstantString(const LilithValue& lilithValue)
{
	std::stringstream ss;

	if (IS_NUMBER(lilithValue))
	{
		ss << lilithValue.number;
	}
	else if (IS_STRING(lilithValue))
	{
		ss << '"' << AS_CPPSTRING(lilithValue) << '"';
	}
	else if (IS_CODE(lilithValue))
	{
		auto code = AS_CODE(lilithValue);
		ss << "code " << code << ": " << code->name;
	}
	else
	{
		DIE << "lilithValueToConstantString: unknow type " << (int)lilithValue.type;
	}
	return ss.str();
}

// Output stream
std::ostream& operator<<(std::ostream& os, const LilithValue& lilithValue)
{
	return os << "LilithValue (" << lilithValueToTypeString(lilithValue)
		      << "); " << lilithValueToConstantString(lilithValue);
}