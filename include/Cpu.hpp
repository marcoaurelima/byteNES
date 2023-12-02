#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <functional>

#include "Memory.hpp"

enum class Flag {
  N = (0x01 << 7), // Negative
  V = (0x01 << 6), // Overflow
  _ = (0x01 << 5), // Ignored
  B = (0x01 << 4), // Break
  D = (0x01 << 3), // Decimal
  I = (0x01 << 2), // Interrupt
  Z = (0x01 << 1), // Zero
  C = (0x01 << 0), // Carry
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
  uint8_t immediate();
  uint8_t zeropage();
  uint8_t zeropageX();
  uint8_t absolute();
  uint8_t absoluteX();
  uint8_t absoluteY();
  uint8_t indirectX();
  uint8_t indirectY();

  // Funções de verificação de ativação de Flags após a operação
  void flagActivationN(uint8_t value);
  void flagActivationV(uint8_t value_orig, uint8_t value_new);
  void flagActivationB(uint8_t value_orig, uint8_t value_new);
  void flagActivationD(uint8_t value_orig, uint8_t value_new);
  void flagActivationI(uint8_t value_orig, uint8_t value_new);
  void flagActivationZ(uint8_t value);
  void flagActivationC(uint8_t value);

  // Implementações das instruções
  // ADC (ADd with Carry)
  void adc_flags_handler(uint8_t value_orig, uint8_t value_new);
  void adc(uint8_t (Cpu::*AddressingMode)());
  void adc_im();
  void adc_zp();
  void adc_zpx();
  void adc_abs();
  void adc_absx();
  void adc_absy();
  void adc_indx();
  void adc_indy();

  // AND (bitwise AND with accumulator)
  void and_flags_handler(uint8_t value_orig, uint8_t value_new);
  void and_im();
  void and_zp();
  void and_zpx();
  void and_abs();
  void and_absx();
  void and_absy();
  void and_indx();
  void and_indy();

  // ASL (Arithmetic Shift Left)
  void asl_flags_handler(uint8_t value_orig, uint8_t value_new);
  void asl_acc();
  void asl_zp();
  void asl_zpx();
  void asl_abs();
  void asl_absx();

  // BIT (test BITs)
  // Branch Instructions
  // BRK (BReaK)
  // CMP (CoMPare accumulator)
  // CPX (ComPare X register)
  // CPY (ComPare Y regiarch?cache=wsyo1o9wyksi3aq9b7bnz8x5ster)
  // DEC (DECrement memory)
  // EOR (bitwise Exclusive OR)
  // Flag (Processor Status) Instructions
  void clc();
  void sec();
  void cli();
  void sei();
  void clv();
  void cld();
  void sed();

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
  void remFlag(Flag flag);
  void incrementPC(uint16_t value);

  // Opcodes Mapping
  std::array<std::function<void()>, 0xFF> opcodeMapping{};
  void fillOpcodeMapping();
};

#endif
