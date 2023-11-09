#ifndef CPU_H
#define CPU_H
#pragma once

#include <iostream>
#include <bitset>
#include <cstdint>

#include "Ram.h"

enum class Flag
{
    N, // Negative
    Z, // Zero
    C, // Carry
    I, // Interrupt
    D, // Decimal
    V, // Overflow
    B, // Break
};

enum
{
    ADC_IM = 0X69,
    ADC_ZP = 0X65,
    ADC_ZPX = 0X75,
    ADC_ABS = 0X6D,
    ADC_ABSX = 0X7D,
    ADC_ABSY = 0X79,
    ADC_INDX = 0X61,
    ADC_INDY = 0X71
};

class Cpu
{
public:
    Cpu();
    Cpu(uint16_t PC, uint8_t SP, uint8_t A, uint8_t Y, uint8_t P);
    ~Cpu();

    // Printar status interno
    void showStatus();

    // Implementações das instruções
    void adc_im(uint8_t value);
    void adc_zp(uint8_t address);
    void adc_zpx(uint8_t address);
    void adc_abs(uint16_t address);
    void adc_absx(uint16_t address);
    void adc_absy(uint16_t address);
    void adc_indx(uint8_t address);
    void adc_indy(uint8_t address);

private:
    // Memoria ram (2Kb)
    Ram* ram;

    // Registradores
    uint16_t PC{0};
    uint8_t SP{0};
    uint8_t A{0};
    uint8_t X{0};
    uint8_t Y{0};
    uint8_t P{0};

    // Flags
    void setFlag(Flag flag);
};

#endif