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
  uint16_t immediate();
  uint16_t zeropage();
  uint16_t zeropageX();
  uint16_t zeropageY();
  uint16_t absolute();
  uint16_t absoluteX();
  uint16_t absoluteY();
  uint16_t indirectX();
  uint16_t indirectY();
  uint16_t accumulator();

  // Funções de verificação de ativação de Flags à partir de valores
  void flagActivationN(uint8_t value);
  void flagActivationV(uint8_t value_orig, uint8_t value_new);
  void flagActivationB();
  void flagActivationD();
  void flagActivationI();
  void flagActivationZ(uint8_t value);
  void flagActivationC(uint16_t value);

  // Implementações das instruções
  // ADC (ADd with Carry)
  void ADC(uint16_t (Cpu::*AddressingMode)());

  // AND (bitwise AND with accumulator)
  void AND(uint16_t (Cpu::*AddressingMode)());

  // ASL (Arithmetic Shift Left)
  void ASL(uint16_t (Cpu::*AddressingMode)());
  void ASL_AC(uint16_t (Cpu::*AddressingMode)());

  // BIT (test BITs)
  // Branch Instructions
  // BRK (BReaK)
  // CMP (CoMPare accumulator)
  // CPX (ComPare X register)
  // CPY (ComPare Y regiarch?cache=wsyo1o9wyksi3aq9b7bnz8x5ster)
  // DEC (DECrement memory)
  // EOR (bitwise Exclusive OR)

  // Flag (Processor Status) Instructions
  void CLC(uint16_t (Cpu::*AddressingMode)());
  void SEC(uint16_t (Cpu::*AddressingMode)());
  void CLI(uint16_t (Cpu::*AddressingMode)());
  void SEI(uint16_t (Cpu::*AddressingMode)());
  void CLV(uint16_t (Cpu::*AddressingMode)());
  void CLD(uint16_t (Cpu::*AddressingMode)());
  void SED(uint16_t (Cpu::*AddressingMode)());

  // INC (INCrement memory)
  // JMP (JuMP)
  // JSR (Jump to SubRoutine)
  // LDA (LoaD Accumulator)
  void LDA(uint16_t (Cpu::*AddressingMode)());
  // LDX (LoaD X register)
  void LDX(uint16_t (Cpu::*AddressingMode)());
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
  // STY (STore Y register)
  

private:
  // Memoria ram (2Kb)
  Memory &memory;

  // Registradores
  uint16_t PC;
  uint8_t SP;
  uint8_t AC;
  uint8_t X;
  uint8_t Y;
  uint8_t SR;

  // Flags
  void setFlag(Flag flag);
  void remFlag(Flag flag);
  void incrementPC(uint16_t value);

  // Opcodes Mapping
  std::array<std::function<void()>, 0xFF> opcodeMapping{};
  void fillOpcodeMapping();
};

#endif
