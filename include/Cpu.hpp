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
// Evento do tipo "page boundary crossed" ocorre quando
// o endereço calculado pelo modo de endereçamento está
// em uma página diferente da atual (PC)
struct MemoryAccessResult {
  uint16_t address; // Endereço calculado
  uint8_t size;     // Tamanho da operação
  bool pageCrossed; // indica se houve evento do tipo "page boundary crossed"
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

  //           cycles: Quantidade de ciclos de CPU usados por um opcode com determinado modo de endereçamento
  // pageChangedCycle: Alguns opcodes, em determinados modos de endereçamento, tem um ou dois ciclos de CPU 
  //                   a mais quando o resultado da operação ultrapassa a página atual. Padrão é zero.
  //                   (+ add 1 cycle if page boundary crossed)               
  // void XXX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);

  // ADC (ADd with Carry)
  void ADC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // AND (bitwise AND with accumulator)
  void AND(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // ASL (Arithmetic Shift Left)
  void ASL(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void ASL_AC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // BIT (test BITs)
  void BIT(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // Branch Instructions
  void BPL(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BMI(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BVC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BVS(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BCC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BCS(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BNE(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void BEQ(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // BRK (BReaK)
  void BRK(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // CMP (CoMPare accumulator)
  void CMP(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // CPX (ComPare X register)
  void CPX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // CPY (ComPare Y register)
  void CPY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // DEC (DECrement memory)
  void DEC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // EOR (bitwise Exclusive OR)
  void EOR(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // Flag (Processor Status) Instructions
  void CLC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void SEC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void CLI(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void SEI(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void CLV(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void CLD(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void SED(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // INC (INCrement memory)
  void INC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // JMP (JuMP)
  void JMP(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // JSR (Jump to SubRoutine)
  void JSR(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // LDA (LoaD Accumulator)
  void LDA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // LDX (LoaD X register)
  void LDX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // LDY (LoaD Y register)
  void LDY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // LSR (Logical Shift Right)
  void LSR(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void LSR_AC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // NOP (No OPeration)
  void NOP(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // ORA (bitwise OR with Accumulator)
  void ORA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // Register Instructions
  void TAX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void TXA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void DEX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void INX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void TAY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void TYA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void DEY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void INY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // ROL (ROtate Left)
  void ROL(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void ROL_AC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // ROR (ROtate Right)
  void ROR(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void ROR_AC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // RTI (ReTurn from Interrupt)
  void RTI(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // RTS (ReTurn from Subroutine)
  void RTS(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // SBC (SuBtract with Carry)
  void SBC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // STA (STore Accumulator)
  void STA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // Stack Instructions
  void TXS(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void TSX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void PHA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void PLA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void PHP(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  void PLP(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // STX (STore X register)
  void STX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);
  // STY (STore Y register)
  void STY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle);

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
