#ifndef MEMORY_H
#define MEMORY_H
#pragma once

#include <iostream>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <fstream>

#define MEMSIZE 0xFFFF

class Memory
{
public:
    Memory();
    ~Memory();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

    void fillRandomData();
    void fillSequencialData();
    void fillZeroData();

    void loadMemoryFromFile(std::string path, uint16_t addrBegin = 0x00);
    std::string getFilePath();
    void reset();
private:
    std::array<uint8_t, MEMSIZE> data;
    std::string filePath;
};

#endif
