#ifndef MEMORY_H
#define MEMORY_H
#pragma once

#include <iostream>
#include <array>
#include <cstdint>
#include <cstdlib>

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

private:
    std::array<uint8_t, MEMSIZE> data;
};

#endif
