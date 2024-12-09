/*
* Lilith value
*/

#ifndef LilithValue_H
#define LilithValue_H

enum class LilithValueType
{
	NUMBER
};

struct LilithValue
{
	LilithValueType type;
	union
	{
		double number;
	};
};

#define NUMBER(value) {.type = LilithValueType::NUMBER, .number = value}

#define AS_NUMBER(lilithValue) ((double)(lilithValue.number))

#endif // LilithValue_H