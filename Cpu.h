#ifndef CPU_H
#define CPU_H

#pragma once

using UINT16 = unsigned short;
using UINT8 = unsigned char;

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
    UINT16 PC{0};
    UINT8  SP{0};
    UINT8  A{0};
    UINT8  Y{0};
    UINT8  P{0};
};

#endif