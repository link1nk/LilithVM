/*
* Lilith value
*/

#ifndef LilithValue_H
#define LilithValue_H

#include <vector>
#include <string>
#include <sstream>
#include <functional>
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
	CODE,
	NATIVE
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

struct LocalVar
{
	std::string name;
	size_t scopeLevel = 0;
};

struct CodeObject : public Object
{
	std::string name;
	std::vector<uint8_t> code;
	std::vector<LilithValue> constants;
	std::vector<LocalVar> locals;
	size_t scopeLevel;

	CodeObject(const std::string& name);

	int getLocalIndex(const std::string& name);
	void addLocal(const std::string& name);
};

using NativeFn = std::function<void()>;

struct NativeObject : public Object
{
	NativeFn function;
	std::string name;
	size_t arity;

	NativeObject(NativeFn function, const std::string& name, size_t arity);
	NativeObject(const std::string& name = "", size_t arity = 0)
		: Object(ObjectType::NATIVE), name(name), arity(arity)
	{}
};

// ------------------------------------------------------------------------------------------------------------------
// CONSTRUCTORS 

#define NUMBER(value) { .type = LilithValueType::NUMBER, .number = value }
#define BOOLEAN(value) { .type = LilithValueType::BOOLEAN, .boolean = value }
#define ALLOC_STRING(value) { .type = LilithValueType::OBJECT, .object = (Object*) new StringObject{ value } }
#define ALLOC_CODE(name) { .type = LilithValueType::OBJECT, .object = (Object*) new CodeObject{ name } }
#define ALLOC_NATIVE(fn, name, arity) {.type = LilithValueType::OBJECT, .object = (Object*) new NativeObject(fn, name, arity)}

// ------------------------------------------------------------------------------------------------------------------
// ACCESSORS

#define AS_NUMBER(lilithValue) ((double)(lilithValue).number)
#define AS_OBJECT(lilithValue) ((Object*)(lilithValue).object)
#define AS_STRING(lilithValue) ((StringObject*)(lilithValue).object)
#define AS_CPPSTRING(lilithValue) (AS_STRING(lilithValue)->string)
#define AS_CODE(lilithValue) ((CodeObject*)(lilithValue).object)
#define AS_BOOLEAN(lilithValue) ((bool)(lilithValue).boolean)
#define AS_NATIVE(lilithValue) ((NativeObject*)(lilithValue).object)

// ------------------------------------------------------------------------------------------------------------------
// TESTERS

#define IS_NUMBER(lilithValue) ((lilithValue).type == LilithValueType::NUMBER)
#define IS_OBJECT(lilithValue) ((lilithValue).type == LilithValueType::OBJECT)
#define IS_OBJECT_TYPE(lilithValue, objectType) (IS_OBJECT(lilithValue) && AS_OBJECT(lilithValue)->type == objectType)
#define IS_STRING(lilithValue) IS_OBJECT_TYPE(lilithValue, ObjectType::STRING)
#define IS_CODE(lilithValue) IS_OBJECT_TYPE(lilithValue, ObjectType::CODE)
#define IS_BOOLEAN(lilithValue) ((lilithValue).type == LilithValueType::BOOLEAN)
#define IS_NATIVE(lilithValue) IS_OBJECT_TYPE(lilithValue, ObjectType::NATIVE)

// String representation used in constants for debug
std::string lilithValueToTypeString(const LilithValue& lilithValue);

// String representation used in constants for debug
std::string lilithValueToConstantString(const LilithValue& lilithValue);

// Output stream
std::ostream& operator<<(std::ostream& os, const LilithValue& lilithValue);

// ------------------------------------------------------------------------------------------------------------------

#endif // LilithValue_H