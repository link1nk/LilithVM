/*
* Instruction set for Lilith VM
*/

#ifndef OpCode_H
#define OpCode_H

#include <cstddef>

constexpr uint8_t OP_HALT{ 0x00 };  // Stops the program
constexpr uint8_t OP_CONST{ 0x01 }; // Pushes a const onto the stack

#endif // OpCode_H