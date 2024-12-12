/*
* Instruction set for Lilith VM
*/

#ifndef OpCode_H
#define OpCode_H

#include <cstddef>

constexpr uint8_t OP_HALT     { 0x00 };  // Stops the program
constexpr uint8_t OP_CONST    { 0x01 };  // Pushes a const onto the stack
constexpr uint8_t OP_ADD      { 0x02 };  // Add instruction
constexpr uint8_t OP_SUB      { 0x03 };  // Sub instruction
constexpr uint8_t OP_DIV      { 0x04 };  // Div instruction
constexpr uint8_t OP_MUL      { 0x05 };  // Mul instruction
constexpr uint8_t OP_COMPARE  { 0x06 };  // Comparison

#endif // OpCode_H