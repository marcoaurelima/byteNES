#ifndef MEMORY_H
#define MEMORY_H
#pragma once

#include <cstdint>

#define MEMSIZE 0xFFFF

class Memory
{
public:
    Memory();
    ~Memory();

    uint8_t read(uint16_t address);
    void write(uint16_t address, uint8_t value);

private:
    uint8_t data[MEMSIZE];
};

#endif