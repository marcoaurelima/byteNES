#ifndef CPU_H
#define CPU_H
#include <array>
#include <functional>
#pragma once

#include <bitset>
#include <cstdint>
#include <iostream>

#include "Memory.hpp"

enum class Flag {
  N, // Negative
  Z, // Zero
  C, // Carry
  I, // Interrupt
  D, // Decimal
  V, // Overflow
  B, // Break
};

enum {
  // ADC (ADd with Carry)
  ADC_IM = 0X69,
  ADC_ZP = 0X65,
  ADC_ZPX = 0X75,
  ADC_ABS = 0X6D,
  ADC_ABSX = 0X7D,
  ADC_ABSY = 0X79,
  ADC_INDX = 0X61,
  ADC_INDY = 0X71,

  // STX (STore X register)
  STX_ZP = 0X86,
  STX_ZPY = 0X96,
  STX_ABS = 0X8E,
};

class Cpu {
public:
  Cpu(Memory &memory, uint16_t PC = 0x00, uint8_t SP = 0x00, uint8_t AC = 0x00,
      uint8_t X = 0x00, uint8_t Y = 0x00, uint8_t SR = 0x00);
  ~Cpu();

  uint16_t getPC();
  uint8_t getSP();
  uint8_t getSR();
  uint8_t getAC();
  uint8_t getX();
  uint8_t getY();
  Memory &getMemory();

  // Executa a próxima instrução do programa
  void next();

  // Reseta a execução
  void reset();

  // Modos de endereçamento
  uint8_t immediate(uint8_t &value);
  uint8_t zeropage(uint8_t address);
  uint8_t absolute(uint16_t address);
  uint8_t indirectX(uint8_t address);
  uint8_t indirectY(uint8_t address);

  // Implementações das instruções
  // ADC (ADd with Carry)
  void adc_im(uint8_t value);
  void adc_zp();
  void adc_zpx();
  void adc_abs();
  void adc_absx();
  void adc_absy();
  void adc_indx();
  void adc_indy();

  // AND (bitwise AND with accumulator)
  // ASL (Arithmetic Shift Left)
  // BIT (test BITs)
  // Branch Instructions
  // BRK (BReaK)
  // CMP (CoMPare accumulator)
  // CPX (ComPare X register)
  // CPY (ComPare Y regiarch?cache=wsyo1o9wyksi3aq9b7bnz8x5ster)
  // DEC (DECrement memory)
  // EOR (bitwise Exclusive OR)
  // Flag (Processor Status) Instructions
  // INC (INCrement memory)
  // JMP (JuMP)
  // JSR (Jump to SubRoutine)
  // LDA (LoaD Accumulator)
  // LDX (LoaD X register)
  // LDY (LoaD Y register)
  // LSR (Logical Shift Right)
  // NOP (No OPeration)
  // ORA (bitwise OR with Accumulator)
  // Register Instructions
  // ROL (ROtate Left)
  // ROR (ROtate Right)
  // RTI (ReTurn from Interrupt)
  // RTS (ReTurn from Subroutine)
  // SBC (SuBtract with Carry)
  // STA (STore Accumulator)
  // Stack Instructions
  // STX (STore X register)
  void stx_zp();
  void stx_zpy();
  void stx_abs();
  // STY (STore Y register)
  void sty_zp();
  void sty_zpx();
  void sty_abs();

private:
  // Memoria ram (2Kb)
  Memory &memory;

  // Registradores
  uint16_t PC;
  uint8_t SP;
  uint8_t AC;
  uint8_t SR;
  uint8_t X;
  uint8_t Y;

  // Flags
  void setFlag(Flag flag);
  void incrementPC(uint8_t value);

  // Opcodes Mapping
  std::array<std::function<void(uint16_t)>, 0xFF> opcodeMapping{};
  void fillOpcodeMapping();
};

#endif
