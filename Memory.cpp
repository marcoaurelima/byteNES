#include "Memory.h"

Memory::Memory()
{
    ram = new uint8_t(SIZE);
    for(int i=0;i<SIZE;i++){
        ram[i] = 0;
    }
}

uint8_t Memory::get(uint16_t address)
{
    return ram[address];
}
void Memory::set(uint16_t address, uint8_t value)
{
    ram[address] = value;
}

Memory::~Memory()
{
}