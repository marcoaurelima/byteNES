#include "Ram.h"

Ram::Ram(const uint16_t size)
{
}

Ram::Ram(const uint16_t size)
{
    data = new uint8_t(size);
    for(int i=0;i<size;i++){
        data[i] = 0;
    }
}

uint8_t Ram::get(uint16_t address)
{
    return data[address];
}
void Ram::set(uint16_t address, uint8_t value)
{
    data[address] = value;
}

Ram::~Ram()
{
    delete []data;
}