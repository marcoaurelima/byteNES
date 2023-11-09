#ifndef RAM_H
#define RAM_H
#pragma once

#include <cstdint>

class Ram
{
public:
    Ram();
    Ram(const uint16_t size);
    ~Ram();

    uint8_t get(uint16_t address);
    void set(uint16_t address, uint8_t value);

private:
    uint8_t *data;
};

#endif