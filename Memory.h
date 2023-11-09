#ifndef MEMORY_H
#define MEMORY_H
#pragma once

#include <cstdint>

class Memory
{
public:
    Memory();
    ~Memory();

    uint8_t get(uint16_t address);
    void set(uint16_t address, uint8_t value);

private:
    const uint16_t SIZE{2048};
    uint8_t* ram;
};

#endif