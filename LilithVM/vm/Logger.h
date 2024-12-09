/*
* Logger and error reporter
*/

#ifndef Logger_H
#define Logger_H

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

#endif // Logger_H