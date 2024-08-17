#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <sys/types.h>

#include "constants.hpp"

#include "Mem.hpp"

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

enum class ADDR_MODE {
  IMMEDIATE,
  ZEROPAGE,
  ZEROPAGE_X,
  ZEROPAGE_Y,
  ABSOLUTE,
  ABSOLUTE_X,
  ABSOLUTE_Y,
  INDIRECT,
  INDIRECT_X,
  INDIRECT_Y,
  RELATIVE,
  NONE
};

struct opcodeParams {
  ADDR_MODE addrMode;
  uint8_t cycles;
  uint8_t cyclesOnPageCross;
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

  // Mostra o status da CPU no termial
  void showCpuStatus(uint8_t index, bool showOpcodes);

  // Executa a próxima instrução do programa
  uint8_t next();

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
  using CPUCicles = uint8_t;
  // ADC (ADd with Carry)
  CPUCicles ADC(const opcodeParams &params);
  // AND (bitwise AND with accumulator)
  CPUCicles AND(const opcodeParams &params);
  // ASL (Arithmetic Shift Left)
  CPUCicles ASL(const opcodeParams &params);
  CPUCicles ASL_AC(const opcodeParams &params);
  // BIT (test BITs)
  CPUCicles BIT(const opcodeParams &params);
  // Branch Instructions
  CPUCicles BPL(const opcodeParams &params);
  CPUCicles BMI(const opcodeParams &params);
  CPUCicles BVC(const opcodeParams &params);
  CPUCicles BVS(const opcodeParams &params);
  CPUCicles BCC(const opcodeParams &params);
  CPUCicles BCS(const opcodeParams &params);
  CPUCicles BNE(const opcodeParams &params);
  CPUCicles BEQ(const opcodeParams &params);
  // BRK (BReaK)
  CPUCicles BRK(const opcodeParams &params);
  // CMP (CoMPare accumulator)
  CPUCicles CMP(const opcodeParams &params);
  // CPX (ComPare X register)
  CPUCicles CPX(const opcodeParams &params);
  // CPY (ComPare Y register)
  CPUCicles CPY(const opcodeParams &params);
  // DEC (DECrement memory)
  CPUCicles DEC(const opcodeParams &params);
  // EOR (bitwise Exclusive OR)
  CPUCicles EOR(const opcodeParams &params);
  // Flag (Processor Status) Instructions
  CPUCicles CLC(const opcodeParams &params);
  CPUCicles SEC(const opcodeParams &params);
  CPUCicles CLI(const opcodeParams &params);
  CPUCicles SEI(const opcodeParams &params);
  CPUCicles CLV(const opcodeParams &params);
  CPUCicles CLD(const opcodeParams &params);
  CPUCicles SED(const opcodeParams &params);
  // INC (INCrement memory)
  CPUCicles INC(const opcodeParams &params);
  // JMP (JuMP)
  CPUCicles JMP(const opcodeParams &params);
  // JSR (Jump to SubRoutine)
  CPUCicles JSR(const opcodeParams &params);
  // LDA (LoaD Accumulator)
  CPUCicles LDA_old(const opcodeParams &params);
  // LDA (LoaD Accumulator)
  CPUCicles LDA(const opcodeParams &params);
  // LDX (LoaD X register)
  CPUCicles LDX(const opcodeParams &params);
  // LDY (LoaD Y register)
  CPUCicles LDY(const opcodeParams &params);
  // LSR (Logical Shift Right)
  CPUCicles LSR(const opcodeParams &params);
  CPUCicles LSR_AC(const opcodeParams &params);
  // NOP (No OPeration)
  CPUCicles NOP(const opcodeParams &params);
  // ORA (bitwise OR with Accumulator)
  CPUCicles ORA(const opcodeParams &params);
  // Register Instructions
  CPUCicles TAX(const opcodeParams &params);
  CPUCicles TXA(const opcodeParams &params);
  CPUCicles DEX(const opcodeParams &params);
  CPUCicles INX(const opcodeParams &params);
  CPUCicles TAY(const opcodeParams &params);
  CPUCicles TYA(const opcodeParams &params);
  CPUCicles DEY(const opcodeParams &params);
  CPUCicles INY(const opcodeParams &params);
  // ROL (ROtate Left)
  CPUCicles ROL(const opcodeParams &params);
  CPUCicles ROL_AC(const opcodeParams &params);
  // ROR (ROtate Right)
  CPUCicles ROR(const opcodeParams &params);
  CPUCicles ROR_AC(const opcodeParams &params);
  // RTI (ReTurn from Interrupt)
  CPUCicles RTI(const opcodeParams &params);
  // RTS (ReTurn from Subroutine)
  CPUCicles RTS(const opcodeParams &params);
  // SBC (SuBtract with Carry)
  CPUCicles SBC(const opcodeParams &params);
  // STA (STore Accumulator)
  CPUCicles STA(const opcodeParams &params);
  // Stack Instructions
  CPUCicles TXS(const opcodeParams &params);
  CPUCicles TSX(const opcodeParams &params);
  CPUCicles PHA(const opcodeParams &params);
  CPUCicles PLA(const opcodeParams &params);
  CPUCicles PHP(const opcodeParams &params);
  CPUCicles PLP(const opcodeParams &params);
  // STX (STore X register)
  CPUCicles STX(const opcodeParams &params);
  // STY (STore Y register)
  CPUCicles STY(const opcodeParams &params);

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

  using OpcodeHandler = CPUCicles (Cpu::*)(const opcodeParams &);
  MemoryAccessResult getValueAddrMode(ADDR_MODE mode);
  // Opcodes Mapping
  // TODO: Usar std::function para OpcodeHandler aumentaria a perfeormance?
  std::array<OpcodeHandler, 0xFF> opcodeMapping{};
  std::array<opcodeParams, 0xFF> opcodeInfo{};
  void fillOpcodeMapping();

  // Opcodes Array names
  std::array<std::string, 0xFF> opcodesNames{opcodesList};

  // Contador de operações (para debugar)
  uint64_t count{};

  void generateRandomIn0xFE();
};

#endif
