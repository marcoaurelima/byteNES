#include "Cpu.h"

Cpu::Cpu()
{
}

Cpu::~Cpu()
{
}

void Cpu::setFlag(Flag flag)
{
    switch (flag)
    {
    case Flag::N:
        P = P | 0b10000000;
        break;
    case Flag::V:
        P = P | 0b01000000;
        break;
    case Flag::B:
        P = P | 0b00010000;
        break;
    case Flag::D:
        P = P | 0b00001000;
        break;
    case Flag::I:
        P = P | 0b00000100;
        break;
    case Flag::Z:
        P = P | 0b00000010;
        break;
    case Flag::C:
        P = P | 0b00000001;
        break;
    default:
        break;
    }
}

void Cpu::adc_im(uint8_t value)
{
    uint8_t result = A + value;

    if(result >= 0b10000000)
        setFlag(Flag::N);

    if (result == 0)
        setFlag(Flag::Z);

    if (result <= A)
        setFlag(Flag::C);

    if(((A ^ value) & 0b10000000) && ((A ^ result) & 0b10000000))
        setFlag(Flag::V);

    A = result;
}
void Cpu::adc_zp()
{
}
void Cpu::adc_zpx()
{
}
void Cpu::adc_abs()
{
}
void Cpu::adc_absx()
{
}
void Cpu::adc_absy()
{
}
void Cpu::adc_indx()
{
}
void Cpu::adc_indy()
{
}