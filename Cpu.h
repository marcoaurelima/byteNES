#ifndef CPU_H
#define CPU_H

#pragma once

using UINT16 = unsigned short;
using UINT8 = unsigned char;

enum {
    ADC = 0x00, //add with carry
    AND = 0x00, //and (with accumulator)
    ASL = 0x00, //arithmetic shift left
    BCC = 0x00, //branch on carry clear
    BCS = 0x00, //branch on carry set
    BEQ = 0x00, //branch on equal (zero set)
    BIT = 0x00, //bit test
    BMI = 0x00, //branch on minus (negative set)
    BNE = 0x00, //branch on not equal (zero clear)
    BPL = 0x00, //branch on plus (negative clear)
    BRK = 0x00, //break / interrupt
    BVC = 0x00, //branch on overflow clear
    BVS = 0x00, //branch on overflow set
    CLC = 0x00, //clear carry
    CLD = 0x00, //clear decimal
    CLI = 0x00, //clear interrupt disable
    CLV = 0x00, //clear overflow
    CMP = 0x00, //compare (with accumulator)
    CPX = 0x00, //compare with X
    CPY = 0x00, //compare with Y
    DEC = 0x00, //decrement
    DEX = 0x00, //decrement X
    DEY = 0x00, //decrement Y
    EOR = 0x00, //exclusive or (with accumulator)
    INC = 0x00, //increment
    INX = 0x00, //increment X
    INY = 0x00, //increment Y
    JMP = 0x00, //jump
    JSR = 0x00, //jump subroutine
    LDA = 0x00, //load accumulator
    LDX = 0x00, //load X
    LDY = 0x00, //load Y
    LSR = 0x00, //logical shift right
    NOP = 0x00, //no operation
    ORA = 0x00, //or with accumulator
    PHA = 0x00, //push accumulator
    PHP = 0x00, //push processor status (SR)
    PLA = 0x00, //pull accumulator
    PLP = 0x00, //pull processor status (SR)
    ROL = 0x00, //rotate left
    ROR = 0x00, //rotate right
    RTI = 0x00, //return from interrupt
    RTS = 0x00, //return from subroutine
    SBC = 0x00, //subtract with carry
    SEC = 0x00, //set carry
    SED = 0x00, //set decimal
    SEI = 0x00, //set interrupt disable
    STA = 0x00, //store accumulator
    STX = 0x00, //store X
    STY = 0x00, //store Y
    TAX = 0x00, //transfer accumulator to X
    TAY = 0x00, //transfer accumulator to Y
    TSX = 0x00, //transfer stack pointer to X
    TXA = 0x00, //transfer X to accumulator
    TXS = 0x00, //transfer X to stack pointer
    TYA = 0x00, //transfer Y to accumulator
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