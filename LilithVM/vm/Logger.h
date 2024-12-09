/*
* Logger and error reporter
*/

#ifndef __Logger_H
#define __Logger_H

#include <sstream>
#include <iostream>

class ErrorLogMessage : public std::basic_ostringstream<char>
{
public:
	~ErrorLogMessage()
	{
		std::cerr << "Fatal error: " << str().c_str();
		exit(EXIT_FAILURE);
	}
};

#define DIE ErrorLogMessage()

#define log(value) std::cout << #value << " = " << std::hex << (value) << '\n';

#endif // __Logger_H