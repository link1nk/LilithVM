#ifndef LilithFile_H
#define LilithFile_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include "../vm/LilithValue.h"
#include "../vm/Global.h"

class LilithFileWriter
{
private:
    static void writeConstant(std::ofstream& file, const LilithValue& value);
    static void writeString(std::ofstream& file, const std::string& str);
    static void writeVector(std::ofstream& file, const std::vector<uint8_t>& vec);
    static void writeVectorConstants(std::ofstream& file, const std::vector<LilithValue>& constants);
    static void writeLilithValue(std::ofstream& file, const LilithValue& value);

public:
    static void writeToFile(const std::string& fileName, const std::vector<CodeObject*>& codeObjects, const Global& global);
};

class LilithFileReader
{
private:
    static LilithValue readConstant(std::ifstream& file);
    static std::string readString(std::ifstream& file);
    static std::vector<uint8_t> readVector(std::ifstream& file);
    static std::vector<LilithValue> readVectorConstants(std::ifstream& file);
    static LilithValue readLilithValue(std::ifstream& file);
    
public:
    static std::pair<std::vector<CodeObject*>, Global> readFromFile(const std::string& fileName);
};

#endif // LilithFile_H