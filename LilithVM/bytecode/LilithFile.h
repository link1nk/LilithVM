#ifndef LilithFile_H
#define LilithFile_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "../vm/LilithValue.h"

class LilithFileWriter
{
private:
    static void writeConstant(std::ofstream& file, const LilithValue& value);
    static void writeString(std::ofstream& file, const std::string& str);
    static void writeVector(std::ofstream& file, const std::vector<uint8_t>& vec);
    static void writeVectorConstants(std::ofstream& file, const std::vector<LilithValue>& constants);

public:
    static void writeToFile(const std::string& fileName, const std::vector<CodeObject*>& codeObjects);
};

class LilithFileReader
{
private:
    static LilithValue readConstant(std::ifstream& file);
    static std::string readString(std::ifstream& file);
    static std::vector<uint8_t> readVector(std::ifstream& file);
    static std::vector<LilithValue> readVectorConstants(std::ifstream& file);
    
public:
    static std::vector<CodeObject*> readFromFile(const std::string& fileName);
};

#endif // LilithFile_H