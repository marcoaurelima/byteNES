#include "Bus.hpp"

Bus::Bus(Cpu& cpu, Memory& memory) : cpu(cpu), memory(memory)
{
}

Bus::~Bus()
{
}

void Bus::loadMemoryFromFile(std::string path, uint16_t addrBegin)
{
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "Error in open file \"" << path << "\"\n";
        exit(1);
    }

    char byte{};
    uint16_t i = 0;
    while (file.read(&byte, sizeof(char)))
    {
        memory.write((i+addrBegin), byte);
        ++i;
    }

    filePath = path;
}

std::string Bus::getFilePath()
{
    return filePath;
}