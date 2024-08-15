#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <functional>
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

// Constantes de frequencia de clock
const uint64_t KHz = 1000;
const uint64_t MHz = 1000000;
const uint64_t GHz = 1000000000;

// Tipo de retorno dos Addressing Modes
// Evento do tipo "page boundary crossed" ocorre quando
// o endereço calculado pelo modo de endereçamento está
// em uma página diferente da atual (PC)
struct MemoryAccessResult {
  uint16_t address; // Endereço calculado
  uint8_t size;     // Tamanho da operação
  bool pageCrossed; // indica se houve evento do tipo "page boundary crossed"
};

/*using AddrModeHanler = MemoryAccessResult (Cpu::*)();*/
/*using OpcodeHandler = void (Cpu::*)(AddrModeHanler, uint8_t, uint8_t);*/

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

  //           cycles: Quantidade de ciclos de CPU usados por um opcode com
  //           determinado modo de endereçamento
  // pageChangedCycle: Alguns opcodes, em determinados modos de endereçamento,
  // tem um ou dois ciclos de CPU
  //                   a mais quando o resultado da operação ultrapassa a página
  //                   atual. Padrão é zero.
  //                   (+ add 1 cycle if page boundary crossed)
  // void XXX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles,
  // uint8_t pageChangedCycle);

  using CPUCicles = uint8_t;
  // ADC (ADd with Carry)
  CPUCicles ADC(opcodeParams params);
  // AND (bitwise AND with accumulator)
  CPUCicles AND(opcodeParams params);
  // ASL (Arithmetic Shift Left)
  CPUCicles ASL(opcodeParams params);
  CPUCicles ASL_AC(opcodeParams params);
  // BIT (test BITs)
  CPUCicles BIT(opcodeParams params);
  // Branch Instructions
  CPUCicles BPL(opcodeParams params);
  CPUCicles BMI(opcodeParams params);
  CPUCicles BVC(opcodeParams params);
  CPUCicles BVS(opcodeParams params);
  CPUCicles BCC(opcodeParams params);
  CPUCicles BCS(opcodeParams params);
  CPUCicles BNE(opcodeParams params);
  CPUCicles BEQ(opcodeParams params);
  // BRK (BReaK)
  CPUCicles BRK(opcodeParams params);
  // CMP (CoMPare accumulator)
  CPUCicles CMP(opcodeParams params);
  // CPX (ComPare X register)
  CPUCicles CPX(opcodeParams params);
  // CPY (ComPare Y register)
  CPUCicles CPY(opcodeParams params);
  // DEC (DECrement memory)
  CPUCicles DEC(opcodeParams params);
  // EOR (bitwise Exclusive OR)
  CPUCicles EOR(opcodeParams params);
  // Flag (Processor Status) Instructions
  CPUCicles CLC(opcodeParams params);
  CPUCicles SEC(opcodeParams params);
  CPUCicles CLI(opcodeParams params);
  CPUCicles SEI(opcodeParams params);
  CPUCicles CLV(opcodeParams params);
  CPUCicles CLD(opcodeParams params);
  CPUCicles SED(opcodeParams params);
  // INC (INCrement memory)
  CPUCicles INC(opcodeParams params);
  // JMP (JuMP)
  CPUCicles JMP(opcodeParams params);
  // JSR (Jump to SubRoutine)
  CPUCicles JSR(opcodeParams params);
  // LDA (LoaD Accumulator)
  CPUCicles LDA_old(opcodeParams params);
  // LDA (LoaD Accumulator)
  CPUCicles LDA(opcodeParams params);
  // LDX (LoaD X register)
  CPUCicles LDX(opcodeParams params);
  // LDY (LoaD Y register)
  CPUCicles LDY(opcodeParams params);
  // LSR (Logical Shift Right)
  CPUCicles LSR(opcodeParams params);
  CPUCicles LSR_AC(opcodeParams params);
  // NOP (No OPeration)
  CPUCicles NOP(opcodeParams params);
  // ORA (bitwise OR with Accumulator)
  CPUCicles ORA(opcodeParams params);
  // Register Instructions
  CPUCicles TAX(opcodeParams params);
  CPUCicles TXA(opcodeParams params);
  CPUCicles DEX(opcodeParams params);
  CPUCicles INX(opcodeParams params);
  CPUCicles TAY(opcodeParams params);
  CPUCicles TYA(opcodeParams params);
  CPUCicles DEY(opcodeParams params);
  CPUCicles INY(opcodeParams params);
  // ROL (ROtate Left)
  CPUCicles ROL(opcodeParams params);
  CPUCicles ROL_AC(opcodeParams params);
  // ROR (ROtate Right)
  CPUCicles ROR(opcodeParams params);
  CPUCicles ROR_AC(opcodeParams params);
  // RTI (ReTurn from Interrupt)
  CPUCicles RTI(opcodeParams params);
  // RTS (ReTurn from Subroutine)
  CPUCicles RTS(opcodeParams params);
  // SBC (SuBtract with Carry)
  CPUCicles SBC(opcodeParams params);
  // STA (STore Accumulator)
  CPUCicles STA(opcodeParams params);
  // Stack Instructions
  CPUCicles TXS(opcodeParams params);
  CPUCicles TSX(opcodeParams params);
  CPUCicles PHA(opcodeParams params);
  CPUCicles PLA(opcodeParams params);
  CPUCicles PHP(opcodeParams params);
  CPUCicles PLP(opcodeParams params);
  // STX (STore X register)
  CPUCicles STX(opcodeParams params);
  // STY (STore Y register)
  CPUCicles STY(opcodeParams params);

  void setAsmAddress(uint16_t address);

  void setInternalClockValue(uint64_t clock);

  uint64_t getCount();

  void start();

private:
  // Memoria ram (2Kb)
  Memory &memory;

  // Endereço inicial do assembler
  uint16_t asmAddress{};

  // Clock interno do processador (em nanosegundos)
  double clock{};

  // Contador de ciclos.
  // Ele começa com o mesmo valor de `clock` e vai sendo incrementado a
  // medida que as instruções vão gastando ciclos de CPU, até (cyclesCounter >=
  // clock). Quando (cyclesCounter >= clock), se passou 1 segundo e o valor de
  // clock é atribuido novamente a cyclesCounter para reiniciar a contagem de
  // ciclos novamente.
  uint64_t cyclesCounter{};

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
  void useCpuCicles(uint8_t qtd);

  void stackPUSH(uint8_t value);
  uint8_t stackPOP();

  /*using AddrModeHanler = MemoryAccessResult (Cpu::*)();*/
  using OpcodeHandler = CPUCicles (Cpu::*)(opcodeParams);
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
