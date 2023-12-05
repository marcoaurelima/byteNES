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

// O endereço inicial da stack é o endereço 
// imediatamente após o último endereço da
// zero page
const uint16_t STACK_ADDRESS = 0x00; // 0X1000; o valor correto é 0x1000; o valor 0x00 é provisório para debugar e testar na UI.

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
  uint16_t indirect();
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
  void flagActivationC_Sum(uint16_t value);
  void flagActivationC_Sub(uint16_t result, uint8_t value);

  // Implementações das instruções
  // ADC (ADd with Carry)
  void ADC(uint16_t (Cpu::*AddressingMode)());
  // AND (bitwise AND with accumulator)
  void AND(uint16_t (Cpu::*AddressingMode)());
  // ASL (Arithmetic Shift Left)
  void ASL(uint16_t (Cpu::*AddressingMode)());
  void ASL_AC(uint16_t (Cpu::*AddressingMode)());
  // BIT (test BITs)
  void BIT(uint16_t (Cpu::*AddressingMode)());
  // Branch Instructions
  // BRK (BReaK)
  // CMP (CoMPare accumulator)
  void CMP(uint16_t (Cpu::*AddressingMode)());
  // CPX (ComPare X register)
  void CPX(uint16_t (Cpu::*AddressingMode)());
  // CPY (ComPare Y register)
  void CPY(uint16_t (Cpu::*AddressingMode)());
  // DEC (DECrement memory)
  void DEC(uint16_t (Cpu::*AddressingMode)());
  // EOR (bitwise Exclusive OR)
  void EOR(uint16_t (Cpu::*AddressingMode)());
  // Flag (Processor Status) Instructions
  void CLC(uint16_t (Cpu::*AddressingMode)());
  void SEC(uint16_t (Cpu::*AddressingMode)());
  void CLI(uint16_t (Cpu::*AddressingMode)());
  void SEI(uint16_t (Cpu::*AddressingMode)());
  void CLV(uint16_t (Cpu::*AddressingMode)());
  void CLD(uint16_t (Cpu::*AddressingMode)());
  void SED(uint16_t (Cpu::*AddressingMode)());
  // INC (INCrement memory)
  void INC(uint16_t (Cpu::*AddressingMode)());
  // JMP (JuMP)
  void JMP(uint16_t (Cpu::*AddressingMode)());
  // JSR (Jump to SubRoutine)
  void JSR(uint16_t (Cpu::*AddressingMode)());
  // LDA (LoaD Accumulator)
  void LDA(uint16_t (Cpu::*AddressingMode)());
  // LDX (LoaD X register)
  void LDX(uint16_t (Cpu::*AddressingMode)());
  // LDY (LoaD Y register)
  void LDY(uint16_t (Cpu::*AddressingMode)());
  // LSR (Logical Shift Right)
  void LSR(uint16_t (Cpu::*AddressingMode)());
  void LSR_AC(uint16_t (Cpu::*AddressingMode)());
  // NOP (No OPeration)
  void NOP(uint16_t (Cpu::*AddressingMode)());
  // ORA (bitwise OR with Accumulator)
  void ORA(uint16_t (Cpu::*AddressingMode)());
  // Register Instructions
  void TAX(uint16_t (Cpu::*AddressingMode)());
  void TXA(uint16_t (Cpu::*AddressingMode)());
  void DEX(uint16_t (Cpu::*AddressingMode)());
  void INX(uint16_t (Cpu::*AddressingMode)());
  void TAY(uint16_t (Cpu::*AddressingMode)());
  void TYA(uint16_t (Cpu::*AddressingMode)());
  void DEY(uint16_t (Cpu::*AddressingMode)());
  void INY(uint16_t (Cpu::*AddressingMode)());
  // ROL (ROtate Left)
  void ROL(uint16_t (Cpu::*AddressingMode)());
  void ROL_AC(uint16_t (Cpu::*AddressingMode)());
  // ROR (ROtate Right)
  void ROR(uint16_t (Cpu::*AddressingMode)());
  void ROR_AC(uint16_t (Cpu::*AddressingMode)());
  // RTI (ReTurn from Interrupt)
  // RTS (ReTurn from Subroutine)
  // SBC (SuBtract with Carry)
  void SBC(uint16_t (Cpu::*AddressingMode)());
  // STA (STore Accumulator)
  void STA(uint16_t (Cpu::*AddressingMode)());
  // Stack Instructions
  void TXS(uint16_t (Cpu::*AddressingMode)());
  void TSX(uint16_t (Cpu::*AddressingMode)());
  void PHA(uint16_t (Cpu::*AddressingMode)());
  void PLA(uint16_t (Cpu::*AddressingMode)());
  void PHP(uint16_t (Cpu::*AddressingMode)());
  void PLP(uint16_t (Cpu::*AddressingMode)());
  // STX (STore X register)
  void STX(uint16_t (Cpu::*AddressingMode)());
  // STY (STore Y register)
  void STY(uint16_t (Cpu::*AddressingMode)());

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
  bool chkFlag(Flag flag);
  void incrementPC(uint16_t value);
  void decrementPC(uint16_t value);
  void incrementSP();
  void decrementSP();

  // Opcodes Mapping
  std::array<std::function<void()>, 0xFF> opcodeMapping{};
  void fillOpcodeMapping();
};

#endif
