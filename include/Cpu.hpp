#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <functional>

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

  //           cycles: Quantidade de ciclos de CPU usados por um opcode com
  //           determinado modo de endereçamento
  // pageChangedCycle: Alguns opcodes, em determinados modos de endereçamento,
  // tem um ou dois ciclos de CPU
  //                   a mais quando o resultado da operação ultrapassa a página
  //                   atual. Padrão é zero.
  //                   (+ add 1 cycle if page boundary crossed)
  // void XXX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles,
  // uint8_t pageChangedCycle);

  // ADC (ADd with Carry)
  void ADC(opcodeParams params);
  // AND (bitwise AND with accumulator)
  void AND(opcodeParams params);
  // ASL (Arithmetic Shift Left)
  void ASL(opcodeParams params);
  void ASL_AC(opcodeParams params);
  // BIT (test BITs)
  void BIT(opcodeParams params);
  // Branch Instructions
  void BPL(opcodeParams params);
  void BMI(opcodeParams params);
  void BVC(opcodeParams params);
  void BVS(opcodeParams params);
  void BCC(opcodeParams params);
  void BCS(opcodeParams params);
  void BNE(opcodeParams params);
  void BEQ(opcodeParams params);
  // BRK (BReaK)
  void BRK(opcodeParams params);
  // CMP (CoMPare accumulator)
  void CMP(opcodeParams params);
  // CPX (ComPare X register)
  void CPX(opcodeParams params);
  // CPY (ComPare Y register)
  void CPY(opcodeParams params);
  // DEC (DECrement memory)
  void DEC(opcodeParams params);
  // EOR (bitwise Exclusive OR)
  void EOR(opcodeParams params);
  // Flag (Processor Status) Instructions
  void CLC(opcodeParams params);
  void SEC(opcodeParams params);
  void CLI(opcodeParams params);
  void SEI(opcodeParams params);
  void CLV(opcodeParams params);
  void CLD(opcodeParams params);
  void SED(opcodeParams params);
  // INC (INCrement memory)
  void INC(opcodeParams params);
  // JMP (JuMP)
  void JMP(opcodeParams params);
  // JSR (Jump to SubRoutine)
  void JSR(opcodeParams params);
  // LDA (LoaD Accumulator)
  void LDA_old(opcodeParams params);
  // LDA (LoaD Accumulator)
  void LDA(opcodeParams params);
  // LDX (LoaD X register)
  void LDX(opcodeParams params);
  // LDY (LoaD Y register)
  void LDY(opcodeParams params);
  // LSR (Logical Shift Right)
  void LSR(opcodeParams params);
  void LSR_AC(opcodeParams params);
  // NOP (No OPeration)
  void NOP(opcodeParams params);
  // ORA (bitwise OR with Accumulator)
  void ORA(opcodeParams params);
  // Register Instructions
  void TAX(opcodeParams params);
  void TXA(opcodeParams params);
  void DEX(opcodeParams params);
  void INX(opcodeParams params);
  void TAY(opcodeParams params);
  void TYA(opcodeParams params);
  void DEY(opcodeParams params);
  void INY(opcodeParams params);
  // ROL (ROtate Left)
  void ROL(opcodeParams params);
  void ROL_AC(opcodeParams params);
  // ROR (ROtate Right)
  void ROR(opcodeParams params);
  void ROR_AC(opcodeParams params);
  // RTI (ReTurn from Interrupt)
  void RTI(opcodeParams params);
  // RTS (ReTurn from Subroutine)
  void RTS(opcodeParams params);
  // SBC (SuBtract with Carry)
  void SBC(opcodeParams params);
  // STA (STore Accumulator)
  void STA(opcodeParams params);
  // Stack Instructions
  void TXS(opcodeParams params);
  void TSX(opcodeParams params);
  void PHA(opcodeParams params);
  void PLA(opcodeParams params);
  void PHP(opcodeParams params);
  void PLP(opcodeParams params);
  // STX (STore X register)
  void STX(opcodeParams params);
  // STY (STore Y register)
  void STY(opcodeParams params);

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
  using OpcodeHandler = void (Cpu::*)(opcodeParams);
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
