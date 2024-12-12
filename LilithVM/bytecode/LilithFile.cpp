#include "LilithFile.h"

void LilithFileWriter::writeConstant(std::ofstream& file, const LilithValue& value)
{
    file.write(reinterpret_cast<const char*>(&value.type), sizeof(value.type));
    switch (value.type)
    {
    case LilithValueType::NUMBER:
        file.write(reinterpret_cast<const char*>(&value.number), sizeof(value.number));
        break;
    case LilithValueType::OBJECT:
    {
        ObjectType objectType = value.object->type;
        file.write(reinterpret_cast<const char*>(&objectType), sizeof(objectType));

        switch (objectType)
        {
        case ObjectType::STRING:
        {
            auto stringObject = static_cast<StringObject*>(value.object);
            writeString(file, stringObject->string); 
        }
        break;

        case ObjectType::CODE:
        {
            auto codeObject = static_cast<CodeObject*>(value.object);
            writeString(file, codeObject->name);
            writeVector(file, codeObject->code); 
            writeVectorConstants(file, codeObject->constants);
        }
        break;
        }
    }
    break;
    case LilithValueType::BOOLEAN:
        file.write(reinterpret_cast<const char*>(&value.boolean), sizeof(value.boolean));
        break;
    }
}

void LilithFileWriter::writeString(std::ofstream& file, const std::string& str)
{
    size_t length = str.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(str.data(), length);
}

void LilithFileWriter::writeVector(std::ofstream& file, const std::vector<uint8_t>& vec)
{
    size_t size = vec.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(vec.data()), size);
}

void LilithFileWriter::writeVectorConstants(std::ofstream& file, const std::vector<LilithValue>& constants)
{
    size_t count = constants.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& constant : constants)
    {
        writeConstant(file, constant);
    }
}

void LilithFileWriter::writeToFile(const std::string& fileName, const std::vector<CodeObject*>& codeObjects)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing: " + fileName);
    }

    size_t codeObjectCount = codeObjects.size();
    file.write(reinterpret_cast<const char*>(&codeObjectCount), sizeof(codeObjectCount));

    for (const auto* codeObject : codeObjects)
    {
        writeString(file, codeObject->name);  
        writeVector(file, codeObject->code); 
        writeVectorConstants(file, codeObject->constants);
    }
}

LilithValue LilithFileReader::readConstant(std::ifstream& file)
{
    LilithValue value;
    file.read(reinterpret_cast<char*>(&value.type), sizeof(value.type));
    switch (value.type)
    {
    case LilithValueType::NUMBER:
        file.read(reinterpret_cast<char*>(&value.number), sizeof(value.number));
        break;
    case LilithValueType::OBJECT:
    {
        ObjectType objectType;
        file.read(reinterpret_cast<char*>(&objectType), sizeof(objectType));

        switch (objectType)
        {
        case ObjectType::STRING:
        {
            auto str = readString(file);
            value.object = new StringObject(str);
        }
        break;

        case ObjectType::CODE:
        {
            auto name = readString(file);
            auto code = readVector(file);
            auto constants = readVectorConstants(file);
            auto* codeObject = new CodeObject(name);
            codeObject->code = std::move(code);
            codeObject->constants = std::move(constants);
            value.object = codeObject;
        }
        break;
        }
    }
    break;
    case LilithValueType::BOOLEAN:
        file.read(reinterpret_cast<char*>(&value.boolean), sizeof(value.boolean));
        break;
    }
    return value;
}

std::string LilithFileReader::readString(std::ifstream& file)
{
    size_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    std::string str(length, '\0');
    file.read(&str[0], length);
    return str;
}

std::vector<uint8_t> LilithFileReader::readVector(std::ifstream& file)
{
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<uint8_t> vec(size);
    file.read(reinterpret_cast<char*>(vec.data()), size);
    return vec;
}

std::vector<LilithValue> LilithFileReader::readVectorConstants(std::ifstream& file)
{
    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    std::vector<LilithValue> constants(count);
    for (auto& constant : constants)
    {
        constant = readConstant(file);
    }
    return constants;
}

std::vector<CodeObject*> LilithFileReader::readFromFile(const std::string& fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for reading: " + fileName);
    }

    size_t codeObjectCount;
    file.read(reinterpret_cast<char*>(&codeObjectCount), sizeof(codeObjectCount));

    std::vector<CodeObject*> codeObjects;
    codeObjects.reserve(codeObjectCount);

    for (size_t i = 0; i < codeObjectCount; ++i)
    {
        auto name = readString(file);
        auto code = readVector(file);
        auto constants = readVectorConstants(file);

        auto* codeObject = new CodeObject(name);
        codeObject->code = std::move(code);
        codeObject->constants = std::move(constants);

        codeObjects.push_back(codeObject);
    }

    return codeObjects;
}
