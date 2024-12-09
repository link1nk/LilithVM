/*
* Lilith value
*/

#ifndef LilithValue_H
#define LilithValue_H

#include <string>

enum class LilithValueType
{
	NUMBER,
	OBJECT
};

enum class ObjectType
{
	STRING
};

struct Object
{
	ObjectType type;

	Object(ObjectType type) :
		type(type)
	{}
};

struct StringObject : public Object
{
	std::string string;

	StringObject(const std::string& str) :
		Object(ObjectType::STRING), string(str)
	{}
};

struct LilithValue
{
	LilithValueType type;
	union
	{
		double number;
		Object* object;
	};
};

// ------------------------------------------------------------------------------
// CONSTRUCTORS 

#define NUMBER(value) { .type = LilithValueType::NUMBER, .number = value }
#define ALLOC_STRING(value) { .type = LilithValueType::OBJECT, .object = new StringObject{ value } }

// ------------------------------------------------------------------------------
// ACCESSORS

#define AS_NUMBER(lilithValue) ((double)(lilithValue.number))
#define AS_OBJECT(lilithValue) ((Object*)(lilithValue.object))
#define AS_STRING(lilithValue) ((StringObject*)(lilithValue).object)
#define AS_CPPSTRING(lilithValue) (AS_STRING(lilithValue)->string)

// ------------------------------------------------------------------------------
// TESTERS

#define IS_NUMBER(lilithValue) ((lilithValue).type == LilithValueType::NUMBER)
#define IS_OBJECT(lilithValue) ((lilithValue).type == LilithValueType::OBJECT)
#define IS_OBJECT_TYPE(lilithValue, objectType) (IS_OBJECT(lilithValue) && AS_OBJECT(lilithValue)->type == objectType)
#define IS_STRING(lilithValue) IS_OBJECT_TYPE(lilithValue, ObjectType::STRING)

#endif // LilithValue_H