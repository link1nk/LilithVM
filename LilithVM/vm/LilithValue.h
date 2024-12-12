/*
* Lilith value
*/

#ifndef LilithValue_H
#define LilithValue_H

#include <vector>
#include <string>
#include <sstream>
#include "Logger.h"

enum class LilithValueType
{
	NUMBER,
	OBJECT,
	BOOLEAN
};

enum class ObjectType
{
	STRING,
	CODE
};

struct Object
{
	ObjectType type;

	Object(ObjectType type) :
		type(type)
	{}
};

struct LilithValue
{
	LilithValueType type;
	union
	{
		double number;
		Object* object;
		bool boolean;
	};
};

struct StringObject : public Object
{
	std::string string;

	StringObject(const std::string& str) :
		Object(ObjectType::STRING), string(str)
	{}
};

struct CodeObject : public Object
{
	std::string name;
	std::vector<uint8_t> code;
	std::vector<LilithValue> constants;

	CodeObject(const std::string& name) :
		Object(ObjectType::CODE), name(name)
	{}
};

// ------------------------------------------------------------------------------------------------------------------
// CONSTRUCTORS 

#define NUMBER(value) { .type = LilithValueType::NUMBER, .number = value }
#define ALLOC_STRING(value) { .type = LilithValueType::OBJECT, .object = (Object*) new StringObject{ value } }
#define ALLOC_CODE(name) { .type = LilithValueType::OBJECT, .object = (Object*) new CodeObject{ name } }
#define BOOLEAN(value) { .type = LilithValueType::BOOLEAN, .boolean = value }

// ------------------------------------------------------------------------------------------------------------------
// ACCESSORS

#define AS_NUMBER(lilithValue) ((double)(lilithValue).number)
#define AS_OBJECT(lilithValue) ((Object*)(lilithValue).object)
#define AS_STRING(lilithValue) ((StringObject*)(lilithValue).object)
#define AS_CPPSTRING(lilithValue) (AS_STRING(lilithValue)->string)
#define AS_CODE(lilithValue) ((CodeObject*)(lilithValue).object)
#define AS_BOOLEAN(lilithValue) ((bool)(lilithValue).boolean)

// ------------------------------------------------------------------------------------------------------------------
// TESTERS

#define IS_NUMBER(lilithValue) ((lilithValue).type == LilithValueType::NUMBER)
#define IS_OBJECT(lilithValue) ((lilithValue).type == LilithValueType::OBJECT)
#define IS_OBJECT_TYPE(lilithValue, objectType) (IS_OBJECT(lilithValue) && AS_OBJECT(lilithValue)->type == objectType)
#define IS_STRING(lilithValue) IS_OBJECT_TYPE(lilithValue, ObjectType::STRING)
#define IS_CODE(lilithValue) IS_OBJECT_TYPE(lilithValue, ObjectType::CODE)
#define IS_BOOLEAN(lilithValue) ((lilithValue).type == LilithValueType::BOOLEAN)

// String representation used in constants for debug
std::string lilithValueToTypeString(const LilithValue& lilithValue);

// String representation used in constants for debug
std::string lilithValueToConstantString(const LilithValue& lilithValue);

// Output stream
std::ostream& operator<<(std::ostream& os, const LilithValue& lilithValue);

// ------------------------------------------------------------------------------------------------------------------

#endif // LilithValue_H