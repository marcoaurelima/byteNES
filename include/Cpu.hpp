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
const uint16_t STACK_ADDRESS = 0x1000;

// Tipo de retorno dos Addressing Modes
struct AMResponse {
  uint16_t address; // Endereço calculado
  uint8_t size;     // Tamanho da operação
};

class Cpu {
public:
  Cpu(Memory &memory);
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
  AMResponse immediate();
  AMResponse zeropage();
  AMResponse zeropageX();
  AMResponse zeropageY();
  AMResponse absolute();
  AMResponse absoluteX();
  AMResponse absoluteY();
  AMResponse indirect();
  AMResponse indirectX();
  AMResponse indirectY();
  AMResponse relative();
  AMResponse accumulator();

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
  void ADC(AMResponse (Cpu::*AddressingMode)());
  // AND (bitwise AND with accumulator)
  void AND(AMResponse (Cpu::*AddressingMode)());
  // ASL (Arithmetic Shift Left)
  void ASL(AMResponse (Cpu::*AddressingMode)());
  void ASL_AC(AMResponse (Cpu::*AddressingMode)());
  // BIT (test BITs)
  void BIT(AMResponse (Cpu::*AddressingMode)());
  // Branch Instructions
  void BPL(AMResponse (Cpu::*AddressingMode)());
  void BMI(AMResponse (Cpu::*AddressingMode)());
  void BVC(AMResponse (Cpu::*AddressingMode)());
  void BVS(AMResponse (Cpu::*AddressingMode)());
  void BCC(AMResponse (Cpu::*AddressingMode)());
  void BCS(AMResponse (Cpu::*AddressingMode)());
  void BNE(AMResponse (Cpu::*AddressingMode)());
  void BEQ(AMResponse (Cpu::*AddressingMode)());
  // BRK (BReaK)
  void BRK(AMResponse (Cpu::*AddressingMode)());
  // CMP (CoMPare accumulator)
  void CMP(AMResponse (Cpu::*AddressingMode)());
  // CPX (ComPare X register)
  void CPX(AMResponse (Cpu::*AddressingMode)());
  // CPY (ComPare Y register)
  void CPY(AMResponse (Cpu::*AddressingMode)());
  // DEC (DECrement memory)
  void DEC(AMResponse (Cpu::*AddressingMode)());
  // EOR (bitwise Exclusive OR)
  void EOR(AMResponse (Cpu::*AddressingMode)());
  // Flag (Processor Status) Instructions
  void CLC(AMResponse (Cpu::*AddressingMode)());
  void SEC(AMResponse (Cpu::*AddressingMode)());
  void CLI(AMResponse (Cpu::*AddressingMode)());
  void SEI(AMResponse (Cpu::*AddressingMode)());
  void CLV(AMResponse (Cpu::*AddressingMode)());
  void CLD(AMResponse (Cpu::*AddressingMode)());
  void SED(AMResponse (Cpu::*AddressingMode)());
  // INC (INCrement memory)
  void INC(AMResponse (Cpu::*AddressingMode)());
  // JMP (JuMP)
  void JMP(AMResponse (Cpu::*AddressingMode)());
  // JSR (Jump to SubRoutine)
  void JSR(AMResponse (Cpu::*AddressingMode)());
  // LDA (LoaD Accumulator)
  void LDA(AMResponse (Cpu::*AddressingMode)());
  // LDX (LoaD X register)
  void LDX(AMResponse (Cpu::*AddressingMode)());
  // LDY (LoaD Y register)
  void LDY(AMResponse (Cpu::*AddressingMode)());
  // LSR (Logical Shift Right)
  void LSR(AMResponse (Cpu::*AddressingMode)());
  void LSR_AC(AMResponse (Cpu::*AddressingMode)());
  // NOP (No OPeration)
  void NOP(AMResponse (Cpu::*AddressingMode)());
  // ORA (bitwise OR with Accumulator)
  void ORA(AMResponse (Cpu::*AddressingMode)());
  // Register Instructions
  void TAX(AMResponse (Cpu::*AddressingMode)());
  void TXA(AMResponse (Cpu::*AddressingMode)());
  void DEX(AMResponse (Cpu::*AddressingMode)());
  void INX(AMResponse (Cpu::*AddressingMode)());
  void TAY(AMResponse (Cpu::*AddressingMode)());
  void TYA(AMResponse (Cpu::*AddressingMode)());
  void DEY(AMResponse (Cpu::*AddressingMode)());
  void INY(AMResponse (Cpu::*AddressingMode)());
  // ROL (ROtate Left)
  void ROL(AMResponse (Cpu::*AddressingMode)());
  void ROL_AC(AMResponse (Cpu::*AddressingMode)());
  // ROR (ROtate Right)
  void ROR(AMResponse (Cpu::*AddressingMode)());
  void ROR_AC(AMResponse (Cpu::*AddressingMode)());
  // RTI (ReTurn from Interrupt)
  void RTI(AMResponse (Cpu::*AddressingMode)());
  // RTS (ReTurn from Subroutine)
  void RTS(AMResponse (Cpu::*AddressingMode)());
  // SBC (SuBtract with Carry)
  void SBC(AMResponse (Cpu::*AddressingMode)());
  // STA (STore Accumulator)
  void STA(AMResponse (Cpu::*AddressingMode)());
  // Stack Instructions
  void TXS(AMResponse (Cpu::*AddressingMode)());
  void TSX(AMResponse (Cpu::*AddressingMode)());
  void PHA(AMResponse (Cpu::*AddressingMode)());
  void PLA(AMResponse (Cpu::*AddressingMode)());
  void PHP(AMResponse (Cpu::*AddressingMode)());
  void PLP(AMResponse (Cpu::*AddressingMode)());
  // STX (STore X register)
  void STX(AMResponse (Cpu::*AddressingMode)());
  // STY (STore Y register)
  void STY(AMResponse (Cpu::*AddressingMode)());

  void setAsmAddress(uint16_t address);

private:
  // Memoria ram (2Kb)
  Memory &memory;

  // Endereço inicial do assembler
  uint16_t asmAddress{};

  // Registradores
  uint16_t PC{};
  uint8_t SP{0xFF};
  uint8_t AC{};
  uint8_t X{};
  uint8_t Y{};
  uint8_t SR{0x30};

  // Flags
  void setFlag(Flag flag);
  void remFlag(Flag flag);
  bool chkFlag(Flag flag);
  void incrementPC(uint16_t value);
  void decrementPC(uint16_t value);

  void stackPUSH(uint8_t value);
  uint8_t stackPOP();

  // Opcodes Mapping
  std::array<std::function<void()>, 0xFF> opcodeMapping{};
  void fillOpcodeMapping();
};

#endif
