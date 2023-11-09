#ifndef CPU_H
#define CPU_H
#pragma once

#include <cstdint>

enum class Flag {
    N, // Negative
    Z, // Zero
    C,  // Carry
    I, // Interrupt
    D, // Decimal
    V, // Overflow
    B, // Break
};

enum {
    ADC_IM   = 0X69,
    ADC_ZP   = 0X65,
    ADC_ZPX  = 0X75,
    ADC_ABS  = 0X6D,
    ADC_ABSX = 0X7D,
    ADC_ABSY = 0X79,
    ADC_INDX = 0X61,
    ADC_INDY = 0X71
};


class Cpu
{
public:
    Cpu();
    ~Cpu();

private:
    // Registradores
    uint16_t PC{0};
    uint8_t  SP{0};
    uint8_t  A{0};
    uint8_t  Y{0};
    uint8_t  P{0};

    // Flags
    void setFlag(Flag flag);


    // Implementações das instruções
    void adc_im(uint8_t value);
    void adc_zp();
    void adc_zpx();
    void adc_abs();
    void adc_absx();
    void adc_absy();
    void adc_indx();
    void adc_indy();
};

#endif