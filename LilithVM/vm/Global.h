/*
* Global object
*/

#ifndef Global_H
#define Global_H

#include <vector>
#include <string>
#include "LilithValue.h"

class Global
{
	struct GlobalVar
	{
		std::string name;
		LilithValue value;
	};

private:
	std::vector<GlobalVar> globals; // Global variables and functions

public:
	Global() = default;

	GlobalVar& get(size_t index);                             // Returns a global
	const std::vector<Global::GlobalVar>& getGlobals() const; // Returns the vector of global
	void set(size_t index, const LilithValue& value);         // Sets a global
	int getGlobalIndex(const std::string& name);              // Get local index
	bool exists(const std::string& name);                     // Wheater a global variable exists
	void define(const std::string& name);                     // Registers a global
	void addConst(const std::string& name, double value);     // Adds a global constant
};

inline Global::GlobalVar& Global::get(size_t index)
{ return globals[index]; }

inline const std::vector<Global::GlobalVar>& Global::getGlobals() const
{ return globals; }

inline bool Global::exists(const std::string& name)
{ return getGlobalIndex(name) != -1; }



#endif // Global_H