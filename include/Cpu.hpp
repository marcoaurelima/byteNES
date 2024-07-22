#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <functional>

#include "constants.hpp"

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

// Parar quando tiver BRK
const bool STOP_BRK = false;

// Tipo de retorno dos Addressing Modes
struct MemoryAccessResult {
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
  MemoryAccessResult immediate();
  MemoryAccessResult zeropage();
  MemoryAccessResult zeropageX();
  MemoryAccessResult zeropageY();
  MemoryAccessResult absolute();
  MemoryAccessResult absoluteX();
  MemoryAccessResult absoluteY();
  MemoryAccessResult indirect();
  MemoryAccessResult indirectX();
  MemoryAccessResult indirectY();
  MemoryAccessResult relative();
  MemoryAccessResult accumulator();

  // Funções de verificação de ativação de Flags à partir de valores
  void flagActivationN(uint8_t value);
  void flagActivationV(uint8_t value_orig, uint8_t value_new);
  void flagActivationB();
  void flagActivationD();
  void flagActivationI();
  void flagActivationZ(uint8_t value);
  void flagActivationC_ovflw(uint16_t value);
  void flagActivationC_unflw(uint16_t value, uint16_t result);
  void flagActivationCMP(uint16_t value_1, uint8_t value_2);

  // Implementações das instruções
  // ADC (ADd with Carry)
  void ADC(MemoryAccessResult (Cpu::*AddressingMode)());
  // AND (bitwise AND with accumulator)
  void AND(MemoryAccessResult (Cpu::*AddressingMode)());
  // ASL (Arithmetic Shift Left)
  void ASL(MemoryAccessResult (Cpu::*AddressingMode)());
  void ASL_AC(MemoryAccessResult (Cpu::*AddressingMode)());
  // BIT (test BITs)
  void BIT(MemoryAccessResult (Cpu::*AddressingMode)());
  // Branch Instructions
  void BPL(MemoryAccessResult (Cpu::*AddressingMode)());
  void BMI(MemoryAccessResult (Cpu::*AddressingMode)());
  void BVC(MemoryAccessResult (Cpu::*AddressingMode)());
  void BVS(MemoryAccessResult (Cpu::*AddressingMode)());
  void BCC(MemoryAccessResult (Cpu::*AddressingMode)());
  void BCS(MemoryAccessResult (Cpu::*AddressingMode)());
  void BNE(MemoryAccessResult (Cpu::*AddressingMode)());
  void BEQ(MemoryAccessResult (Cpu::*AddressingMode)());
  // BRK (BReaK)
  void BRK(MemoryAccessResult (Cpu::*AddressingMode)());
  // CMP (CoMPare accumulator)
  void CMP(MemoryAccessResult (Cpu::*AddressingMode)());
  // CPX (ComPare X register)
  void CPX(MemoryAccessResult (Cpu::*AddressingMode)());
  // CPY (ComPare Y register)
  void CPY(MemoryAccessResult (Cpu::*AddressingMode)());
  // DEC (DECrement memory)
  void DEC(MemoryAccessResult (Cpu::*AddressingMode)());
  // EOR (bitwise Exclusive OR)
  void EOR(MemoryAccessResult (Cpu::*AddressingMode)());
  // Flag (Processor Status) Instructions
  void CLC(MemoryAccessResult (Cpu::*AddressingMode)());
  void SEC(MemoryAccessResult (Cpu::*AddressingMode)());
  void CLI(MemoryAccessResult (Cpu::*AddressingMode)());
  void SEI(MemoryAccessResult (Cpu::*AddressingMode)());
  void CLV(MemoryAccessResult (Cpu::*AddressingMode)());
  void CLD(MemoryAccessResult (Cpu::*AddressingMode)());
  void SED(MemoryAccessResult (Cpu::*AddressingMode)());
  // INC (INCrement memory)
  void INC(MemoryAccessResult (Cpu::*AddressingMode)());
  // JMP (JuMP)
  void JMP(MemoryAccessResult (Cpu::*AddressingMode)());
  // JSR (Jump to SubRoutine)
  void JSR(MemoryAccessResult (Cpu::*AddressingMode)());
  // LDA (LoaD Accumulator)
  void LDA(MemoryAccessResult (Cpu::*AddressingMode)());
  // LDX (LoaD X register)
  void LDX(MemoryAccessResult (Cpu::*AddressingMode)());
  // LDY (LoaD Y register)
  void LDY(MemoryAccessResult (Cpu::*AddressingMode)());
  // LSR (Logical Shift Right)
  void LSR(MemoryAccessResult (Cpu::*AddressingMode)());
  void LSR_AC(MemoryAccessResult (Cpu::*AddressingMode)());
  // NOP (No OPeration)
  void NOP(MemoryAccessResult (Cpu::*AddressingMode)());
  // ORA (bitwise OR with Accumulator)
  void ORA(MemoryAccessResult (Cpu::*AddressingMode)());
  // Register Instructions
  void TAX(MemoryAccessResult (Cpu::*AddressingMode)());
  void TXA(MemoryAccessResult (Cpu::*AddressingMode)());
  void DEX(MemoryAccessResult (Cpu::*AddressingMode)());
  void INX(MemoryAccessResult (Cpu::*AddressingMode)());
  void TAY(MemoryAccessResult (Cpu::*AddressingMode)());
  void TYA(MemoryAccessResult (Cpu::*AddressingMode)());
  void DEY(MemoryAccessResult (Cpu::*AddressingMode)());
  void INY(MemoryAccessResult (Cpu::*AddressingMode)());
  // ROL (ROtate Left)
  void ROL(MemoryAccessResult (Cpu::*AddressingMode)());
  void ROL_AC(MemoryAccessResult (Cpu::*AddressingMode)());
  // ROR (ROtate Right)
  void ROR(MemoryAccessResult (Cpu::*AddressingMode)());
  void ROR_AC(MemoryAccessResult (Cpu::*AddressingMode)());
  // RTI (ReTurn from Interrupt)
  void RTI(MemoryAccessResult (Cpu::*AddressingMode)());
  // RTS (ReTurn from Subroutine)
  void RTS(MemoryAccessResult (Cpu::*AddressingMode)());
  // SBC (SuBtract with Carry)
  void SBC(MemoryAccessResult (Cpu::*AddressingMode)());
  // STA (STore Accumulator)
  void STA(MemoryAccessResult (Cpu::*AddressingMode)());
  // Stack Instructions
  void TXS(MemoryAccessResult (Cpu::*AddressingMode)());
  void TSX(MemoryAccessResult (Cpu::*AddressingMode)());
  void PHA(MemoryAccessResult (Cpu::*AddressingMode)());
  void PLA(MemoryAccessResult (Cpu::*AddressingMode)());
  void PHP(MemoryAccessResult (Cpu::*AddressingMode)());
  void PLP(MemoryAccessResult (Cpu::*AddressingMode)());
  // STX (STore X register)
  void STX(MemoryAccessResult (Cpu::*AddressingMode)());
  // STY (STore Y register)
  void STY(MemoryAccessResult (Cpu::*AddressingMode)());

  void setAsmAddress(uint16_t address);

  uint64_t getCount();

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

  // Opcodes Array names
  std::array<std::string, 0xFF> opcodesNames{opcodesList};

  // Contador de operações (para debugar)
  uint64_t count{};

  void generateRandomIn0xFE();
};

#endif
