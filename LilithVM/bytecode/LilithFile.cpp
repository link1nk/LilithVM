#include "LilithFile.h"

// Writes a constant value to the file in a serialized format.
void LilithFileWriter::writeConstant(std::ofstream& file, const LilithValue& value)
{
    file.write(reinterpret_cast<const char*>(&value.type), sizeof(value.type));
    switch (value.type)
    {
    // Writes a numeric constant.
    case LilithValueType::NUMBER:
        file.write(reinterpret_cast<const char*>(&value.number), sizeof(value.number));
        break;

    // Handles objects such as strings and code objects.
    case LilithValueType::OBJECT: 
    {
        ObjectType objectType = value.object->type;
        file.write(reinterpret_cast<const char*>(&objectType), sizeof(objectType));

        switch (objectType)
        {
        // Serializes a string object.
        case ObjectType::STRING: 
        {
            auto stringObject = static_cast<StringObject*>(value.object);
            writeString(file, stringObject->string);
        }
        break;

        // Serializes a code object.
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

    // Writes a boolean constant.
    case LilithValueType::BOOLEAN: 
        file.write(reinterpret_cast<const char*>(&value.boolean), sizeof(value.boolean));
        break;
    }
}

// Writes a string to the file with its length and content.
void LilithFileWriter::writeString(std::ofstream& file, const std::string& str)
{
    size_t length = str.size();
    file.write(reinterpret_cast<const char*>(&length), sizeof(length));
    file.write(str.data(), length);
}

// Writes a vector of bytes to the file.
void LilithFileWriter::writeVector(std::ofstream& file, const std::vector<uint8_t>& vec)
{
    size_t size = vec.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(vec.data()), size);
}

// Writes a vector of constants (LilithValue) to the file.
void LilithFileWriter::writeVectorConstants(std::ofstream& file, const std::vector<LilithValue>& constants)
{
    size_t count = constants.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));
    for (const auto& constant : constants)
    {
        writeConstant(file, constant);
    }
}

// Writes a single LilithValue to the file, including its type and data.
void LilithFileWriter::writeLilithValue(std::ofstream& file, const LilithValue& value)
{
    file.write(reinterpret_cast<const char*>(&value.type), sizeof(value.type));

    switch (value.type)
    {
    // Handles numeric values.
    case LilithValueType::NUMBER: 
        file.write(reinterpret_cast<const char*>(&value.number), sizeof(value.number));
        break;

    // Handles object types.
    case LilithValueType::OBJECT: 
        file.write(reinterpret_cast<const char*>(&value.object->type), sizeof(value.object->type));
        if (value.object->type == ObjectType::STRING) // Writes string objects.
        {
            auto* strObj = static_cast<StringObject*>(value.object);
            writeString(file, strObj->string);
        }
        break;

    // Handles boolean values.
    case LilithValueType::BOOLEAN: 
        file.write(reinterpret_cast<const char*>(&value.boolean), sizeof(value.boolean));
        break;
    }
}

// Serializes and writes CodeObjects and global variables to a file.
void LilithFileWriter::writeToFile(const std::string& fileName, const std::vector<CodeObject*>& codeObjects, const Global& global)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file for writing: " + fileName);
    }

    // Write the number of CodeObjects.
    size_t codeObjectCount = codeObjects.size();
    file.write(reinterpret_cast<const char*>(&codeObjectCount), sizeof(codeObjectCount));

    // Serialize each CodeObject.
    for (const auto* codeObject : codeObjects)
    {
        writeString(file, codeObject->name);
        writeVector(file, codeObject->code);
        writeVectorConstants(file, codeObject->constants);
    }

    // Write global variables.
    const auto& globals = global.getGlobals();
    size_t globalCount = globals.size();
    file.write(reinterpret_cast<const char*>(&globalCount), sizeof(globalCount));

    for (const auto& globalVar : globals)
    {
        writeString(file, globalVar.name);       // Serialize variable name.
        writeLilithValue(file, globalVar.value); // Serialize variable value.
    }
}

// Reads a constant value from the file.
LilithValue LilithFileReader::readConstant(std::ifstream& file)
{
    LilithValue value;
    file.read(reinterpret_cast<char*>(&value.type), sizeof(value.type));
    switch (value.type)
    {
    // Reads a numeric constant.
    case LilithValueType::NUMBER: 
        file.read(reinterpret_cast<char*>(&value.number), sizeof(value.number));
        break;

    // Reads an object (string or code).
    case LilithValueType::OBJECT: 
    {
        ObjectType objectType;
        file.read(reinterpret_cast<char*>(&objectType), sizeof(objectType));

        switch (objectType)
        {
        // Reads a string object.
        case ObjectType::STRING: 
        {
            auto str = readString(file);
            value.object = new StringObject(str);
        }
        break;

        // Reads a code object.
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

    // Reads a boolean constant.
    case LilithValueType::BOOLEAN: 
        file.read(reinterpret_cast<char*>(&value.boolean), sizeof(value.boolean));
        break;
    }
    return value;
}

// Reads a string from the file by first reading its length.
std::string LilithFileReader::readString(std::ifstream& file)
{
    size_t length;
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    std::string str(length, '\0');
    file.read(&str[0], length);
    return str;
}

// Reads a vector of bytes from the file.
std::vector<uint8_t> LilithFileReader::readVector(std::ifstream& file)
{
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    std::vector<uint8_t> vec(size);
    file.read(reinterpret_cast<char*>(vec.data()), size);
    return vec;
}

// Reads a vector of constants (LilithValue) from the file.
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

// Reads a single LilithValue from the file.
LilithValue LilithFileReader::readLilithValue(std::ifstream& file)
{
    LilithValue value;
    file.read(reinterpret_cast<char*>(&value.type), sizeof(value.type));

    switch (value.type)
    {
    // Reads a numeric value.
    case LilithValueType::NUMBER: 
        file.read(reinterpret_cast<char*>(&value.number), sizeof(value.number));
        break;

    // Reads an object type.
    case LilithValueType::OBJECT: 
        ObjectType objType;
        file.read(reinterpret_cast<char*>(&objType), sizeof(objType));

        // Reads a string object.
        if (objType == ObjectType::STRING) 
        {
            auto str = readString(file);
            value.object = new StringObject(str);
        }
        break;

    // Reads a boolean value.
    case LilithValueType::BOOLEAN: 
        file.read(reinterpret_cast<char*>(&value.boolean), sizeof(value.boolean));
        break;
    }

    return value;
}

// Reads CodeObjects and global variables from a file.
std::pair<std::vector<CodeObject*>, Global> LilithFileReader::readFromFile(const std::string& fileName)
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

    // Deserialize each CodeObject.
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

    Global global;
    size_t globalCount;
    file.read(reinterpret_cast<char*>(&globalCount), sizeof(globalCount));

    // Deserialize global variables.
    for (size_t i = 0; i < globalCount; ++i)
    {
        auto name = readString(file);
        auto value = readLilithValue(file);
        global.define(name);
        global.set(global.getGlobalIndex(name), value);
    }

    return { std::move(codeObjects), std::move(global) };
}
