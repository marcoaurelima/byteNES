#include "Cpu.hpp"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>
#include <ostream>

Cpu::Cpu(Memory &memory) : memory(memory) { fillOpcodeMapping(); }

Cpu::~Cpu() {}

void Cpu::fillOpcodeMapping() {

  // ADC (ADd with Carry)
  opcodeMapping[0x69] = [this]() { this->ADC(&Cpu::immediate, 2, 0); };
  opcodeMapping[0x65] = [this]() { this->ADC(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x75] = [this]() { this->ADC(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0x6D] = [this]() { this->ADC(&Cpu::absolute, 4, 0); };
  opcodeMapping[0x7D] = [this]() { this->ADC(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0x79] = [this]() { this->ADC(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0x61] = [this]() { this->ADC(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0x71] = [this]() { this->ADC(&Cpu::indirectY, 5, 1); };
  // AND (bitwise AND with accumulator)
  opcodeMapping[0x29] = [this]() { this->AND(&Cpu::immediate, 2, 0); };
  opcodeMapping[0x25] = [this]() { this->AND(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x35] = [this]() { this->AND(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0x2D] = [this]() { this->AND(&Cpu::absolute, 4, 0); };
  opcodeMapping[0x3D] = [this]() { this->AND(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0x39] = [this]() { this->AND(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0x21] = [this]() { this->AND(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0x31] = [this]() { this->AND(&Cpu::indirectY, 5, 1); };
  // ASL (Arithmetic Shift Left)
  opcodeMapping[0x0A] = [this]() { this->ASL_AC(nullptr, 2, 0); };
  opcodeMapping[0x06] = [this]() { this->ASL(&Cpu::zeropage, 5, 0); };
  opcodeMapping[0x16] = [this]() { this->ASL(&Cpu::zeropageX, 6, 0); };
  opcodeMapping[0x0E] = [this]() { this->ASL(&Cpu::absolute, 6, 0); };
  opcodeMapping[0x1E] = [this]() { this->ASL(&Cpu::absoluteX, 7, 0); };
  // BIT (test BITs)
  opcodeMapping[0x24] = [this]() { this->BIT(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x2C] = [this]() { this->BIT(&Cpu::absolute, 4, 0); };
  // Branch Instructions
  opcodeMapping[0x10] = [this]() { this->BPL(&Cpu::relative, 2, 2); };
  opcodeMapping[0x30] = [this]() { this->BMI(&Cpu::relative, 2, 2); };
  opcodeMapping[0x50] = [this]() { this->BVC(&Cpu::relative, 2, 2); };
  opcodeMapping[0x70] = [this]() { this->BVS(&Cpu::relative, 2, 2); };
  opcodeMapping[0x90] = [this]() { this->BCC(&Cpu::relative, 2, 2); };
  opcodeMapping[0xB0] = [this]() { this->BCS(&Cpu::relative, 2, 2); };
  opcodeMapping[0xD0] = [this]() { this->BNE(&Cpu::relative, 2, 2); };
  opcodeMapping[0xF0] = [this]() { this->BEQ(&Cpu::relative, 2, 2); };
  // BRK (BReaK)
  opcodeMapping[0x00] = [this]() { this->BRK(nullptr, 7, 0); };
  // CMP (CoMPare accumulator)
  opcodeMapping[0xC9] = [this]() { this->CMP(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xC5] = [this]() { this->CMP(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xD5] = [this]() { this->CMP(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0xCD] = [this]() { this->CMP(&Cpu::absolute, 4, 0); };
  opcodeMapping[0xDD] = [this]() { this->CMP(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0xD9] = [this]() { this->CMP(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0xC1] = [this]() { this->CMP(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0xD1] = [this]() { this->CMP(&Cpu::indirectY, 5, 1); };
  // CPX (ComPare X register)
  opcodeMapping[0xE0] = [this]() { this->CPX(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xE4] = [this]() { this->CPX(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xEC] = [this]() { this->CPX(&Cpu::absolute, 4, 0); };
  // CPY (ComPare Y register)
  opcodeMapping[0xC0] = [this]() { this->CPY(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xC4] = [this]() { this->CPY(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xCC] = [this]() { this->CPY(&Cpu::absolute, 4, 0); };
  // DEC (DECrement memory)
  opcodeMapping[0xC6] = [this]() { this->DEC(&Cpu::zeropage, 5, 0); };
  opcodeMapping[0xD6] = [this]() { this->DEC(&Cpu::zeropageX, 6, 0); };
  opcodeMapping[0xCE] = [this]() { this->DEC(&Cpu::absolute, 6, 0); };
  opcodeMapping[0xDE] = [this]() { this->DEC(&Cpu::absoluteX, 7, 0); };
  // EOR (bitwise Exclusive OR)
  opcodeMapping[0x49] = [this]() { this->EOR(&Cpu::immediate, 2, 0); };
  opcodeMapping[0x45] = [this]() { this->EOR(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x55] = [this]() { this->EOR(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0x4D] = [this]() { this->EOR(&Cpu::absolute, 4, 0); };
  opcodeMapping[0x5D] = [this]() { this->EOR(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0x59] = [this]() { this->EOR(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0x41] = [this]() { this->EOR(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0x51] = [this]() { this->EOR(&Cpu::indirectY, 5, 1); };
  // Flag (Processor Status) Instructions
  opcodeMapping[0x18] = [this]() { this->CLC(nullptr, 2, 0); };
  opcodeMapping[0x38] = [this]() { this->SEC(nullptr, 2, 0); };
  opcodeMapping[0x58] = [this]() { this->CLI(nullptr, 2, 0); };
  opcodeMapping[0x78] = [this]() { this->SEI(nullptr, 2, 0); };
  opcodeMapping[0xB8] = [this]() { this->CLV(nullptr, 2, 0); };
  opcodeMapping[0xD8] = [this]() { this->CLD(nullptr, 2, 0); };
  opcodeMapping[0xF8] = [this]() { this->SED(nullptr, 2, 0); };
  // INC (INCrement memory)
  opcodeMapping[0xE6] = [this]() { this->INC(&Cpu::zeropage, 5, 0); };
  opcodeMapping[0xF6] = [this]() { this->INC(&Cpu::zeropageX, 6, 0); };
  opcodeMapping[0xEE] = [this]() { this->INC(&Cpu::absolute, 6, 0); };
  opcodeMapping[0xFE] = [this]() { this->INC(&Cpu::absoluteX, 7 , 0); };
  // JMP (JuMP)
  opcodeMapping[0x4C] = [this]() { this->JMP(&Cpu::absolute, 3, 0); };
  opcodeMapping[0x6C] = [this]() { this->JMP(&Cpu::indirect, 5, 0); };
  // JSR (Jump to SubRoutine)
  opcodeMapping[0x20] = [this]() { this->JSR(&Cpu::absolute, 6, 0); };
  // LDA (LoaD Accumulator)
  opcodeMapping[0xA9] = [this]() { this->LDA(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xA5] = [this]() { this->LDA(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xB5] = [this]() { this->LDA(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0xAD] = [this]() { this->LDA(&Cpu::absolute, 4, 0); };
  opcodeMapping[0xBD] = [this]() { this->LDA(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0xB9] = [this]() { this->LDA(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0xA1] = [this]() { this->LDA(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0xB1] = [this]() { this->LDA(&Cpu::indirectY, 5, 1); };
  // LDX (LoaD X register)
  opcodeMapping[0xA2] = [this]() { this->LDX(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xA6] = [this]() { this->LDX(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xB6] = [this]() { this->LDX(&Cpu::zeropageY, 4, 0); };
  opcodeMapping[0xAE] = [this]() { this->LDX(&Cpu::absolute, 4, 0); };
  opcodeMapping[0xBE] = [this]() { this->LDX(&Cpu::absoluteY, 4, 1); };
  // LDY (LoaD Y register)
  opcodeMapping[0xA0] = [this]() { this->LDY(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xA4] = [this]() { this->LDY(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xB4] = [this]() { this->LDY(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0xAC] = [this]() { this->LDY(&Cpu::absolute, 4, 0); };
  opcodeMapping[0xBC] = [this]() { this->LDY(&Cpu::absoluteX, 4, 1); };
  // LSR (Logical Shift Right)
  opcodeMapping[0x4A] = [this]() { this->LSR_AC(nullptr, 2, 0); };
  opcodeMapping[0x46] = [this]() { this->LSR(&Cpu::zeropage, 5, 0); };
  opcodeMapping[0x56] = [this]() { this->LSR(&Cpu::zeropageX, 6, 0); };
  opcodeMapping[0x4E] = [this]() { this->LSR(&Cpu::absolute, 6, 0); };
  opcodeMapping[0x5E] = [this]() { this->LSR(&Cpu::absoluteX, 7, 0); };
  // NOP (No OPeration)
  opcodeMapping[0xEA] = [this]() { this->NOP(nullptr, 2, 0); };
  // ORA (bitwise OR with Accumulator)
  opcodeMapping[0x09] = [this]() { this->ORA(&Cpu::immediate, 2, 0); };
  opcodeMapping[0x05] = [this]() { this->ORA(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x15] = [this]() { this->ORA(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0x0D] = [this]() { this->ORA(&Cpu::absolute, 4, 0); };
  opcodeMapping[0x1D] = [this]() { this->ORA(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0x19] = [this]() { this->ORA(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0x01] = [this]() { this->ORA(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0x11] = [this]() { this->ORA(&Cpu::indirectY, 5, 1); };
  // Register Instructions
  opcodeMapping[0xAA] = [this]() { this->TAX(nullptr, 2, 0); };
  opcodeMapping[0x8A] = [this]() { this->TXA(nullptr, 2, 0); };
  opcodeMapping[0xCA] = [this]() { this->DEX(nullptr, 2, 0); };
  opcodeMapping[0xE8] = [this]() { this->INX(nullptr, 2, 0); };
  opcodeMapping[0xA8] = [this]() { this->TAY(nullptr, 2, 0); };
  opcodeMapping[0x98] = [this]() { this->TYA(nullptr, 2, 0); };
  opcodeMapping[0x88] = [this]() { this->DEY(nullptr, 2, 0); };
  opcodeMapping[0xC8] = [this]() { this->INY(nullptr, 2, 0); };
  // ROL (ROtate Left)
  opcodeMapping[0x2A] = [this]() { this->ROL_AC(nullptr, 2, 0); };
  opcodeMapping[0x26] = [this]() { this->ROL(&Cpu::zeropage, 5, 0); };
  opcodeMapping[0x36] = [this]() { this->ROL(&Cpu::zeropageX, 6, 0); };
  opcodeMapping[0x2E] = [this]() { this->ROL(&Cpu::absolute, 6, 0); };
  opcodeMapping[0x3E] = [this]() { this->ROL(&Cpu::absoluteX, 7, 0); };
  // ROR (ROtate Right)
  opcodeMapping[0x6A] = [this]() { this->ROR_AC(nullptr, 2, 0); };
  opcodeMapping[0x66] = [this]() { this->ROR(&Cpu::zeropage, 5, 0); };
  opcodeMapping[0x76] = [this]() { this->ROR(&Cpu::zeropageX, 6, 0); };
  opcodeMapping[0x6E] = [this]() { this->ROR(&Cpu::absolute, 6, 0); };
  opcodeMapping[0x7E] = [this]() { this->ROR(&Cpu::absoluteX, 7, 0); };
  // RTI (ReTurn from Interrupt)
  opcodeMapping[0x40] = [this]() { this->RTI(nullptr, 6, 0); };
  // RTS (ReTurn from Subroutine)
  opcodeMapping[0x60] = [this]() { this->RTS(nullptr, 6, 0); };
  // SBC (SuBtract with Carry)
  opcodeMapping[0xE9] = [this]() { this->SBC(&Cpu::immediate, 2, 0); };
  opcodeMapping[0xE5] = [this]() { this->SBC(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0xF5] = [this]() { this->SBC(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0xED] = [this]() { this->SBC(&Cpu::absolute, 4, 0); };
  opcodeMapping[0xFD] = [this]() { this->SBC(&Cpu::absoluteX, 4, 1); };
  opcodeMapping[0xF9] = [this]() { this->SBC(&Cpu::absoluteY, 4, 1); };
  opcodeMapping[0xE1] = [this]() { this->SBC(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0xF1] = [this]() { this->SBC(&Cpu::indirectY, 5, 1); };
  // STA (STore Accumulator)
  opcodeMapping[0x85] = [this]() { this->STA(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x95] = [this]() { this->STA(&Cpu::zeropageX, 4, 0); };
  opcodeMapping[0x8D] = [this]() { this->STA(&Cpu::absolute, 4, 0); };
  opcodeMapping[0x9D] = [this]() { this->STA(&Cpu::absoluteX, 5, 0); };
  opcodeMapping[0x99] = [this]() { this->STA(&Cpu::absoluteY, 5, 0); };
  opcodeMapping[0x81] = [this]() { this->STA(&Cpu::indirectX, 6, 0); };
  opcodeMapping[0x91] = [this]() { this->STA(&Cpu::indirectY, 6, 0); };
  // Stack Instructions
  opcodeMapping[0x9A] = [this]() { this->TXS(nullptr, 2, 0); };
  opcodeMapping[0xBA] = [this]() { this->TSX(nullptr, 2, 0); };
  opcodeMapping[0x48] = [this]() { this->PHA(nullptr, 3, 0); };
  opcodeMapping[0x68] = [this]() { this->PLA(nullptr, 4, 0); };
  opcodeMapping[0x08] = [this]() { this->PHP(nullptr, 3, 0); };
  opcodeMapping[0x28] = [this]() { this->PLP(nullptr, 4, 0); };
  // STX (STore X register)
  opcodeMapping[0x86] = [this]() { this->STX(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x96] = [this]() { this->STX(&Cpu::zeropageY, 4, 0); };
  opcodeMapping[0x8E] = [this]() { this->STX(&Cpu::absolute, 4, 0); };
  // STY (STore Y register)
  opcodeMapping[0x84] = [this]() { this->STY(&Cpu::zeropage, 3, 0); };
  opcodeMapping[0x94] = [this]() { this->STY(&Cpu::zeropageY, 4, 0); };
  opcodeMapping[0x8C] = [this]() { this->STY(&Cpu::absolute, 4, 0); };

  srand(time(NULL));
}

void Cpu::setAsmAddress(uint16_t address) {
  asmAddress = address;
  PC = address;
}

void Cpu::setInternalClockValue(uint64_t clock){
  this->clock = clock;
}

Memory &Cpu::getMemory() { return memory; }
uint16_t Cpu::getPC() { return PC; }
uint8_t Cpu::getSP() { return SP; }
uint8_t Cpu::getAC() { return AC; }
uint8_t Cpu::getX() { return X; }
uint8_t Cpu::getY() { return Y; }
uint8_t Cpu::getSR() { return SR; }

uint64_t Cpu::getCount() { return count; }

void Cpu::setFlag(Flag flag) { SR = SR | static_cast<uint8_t>(flag); }
void Cpu::remFlag(Flag flag) { SR = SR & ~(static_cast<uint8_t>(flag)); }
bool Cpu::chkFlag(Flag flag) { return (SR & static_cast<uint8_t>(flag)) != 0; }

void Cpu::incrementPC(uint16_t value) { PC += value; }
void Cpu::decrementPC(uint16_t value) { PC -= value; }

void Cpu::useCpuCicles(uint8_t qtd) {
  cyclesCounter += qtd;

  // Verificar os ciclos
    if (cyclesCounter >= clock) {
      std::cout << "+++++++++ Ciclos esgotados. Atualizando cyclesCounter... +++++++++\n";
      cyclesCounter = 0;
    }
}

void Cpu::stackPUSH(uint8_t value) {
  // a stack tem offset por que ela começa em 10FF,
  // mas SP só armazenda 8 bits
  const uint16_t offset = 0x0100;
  memory.write(SP + offset, value);

  // para debug na zeropage:
  // memory.write(SP, value);

  SP -= 0x01;
}

uint8_t Cpu::stackPOP() {
  SP += 0x01;
  const uint16_t offset = 0x0100;
  int8_t value = memory.read(SP + offset);
  return value;
}

// Específico do emulador do endereço
// https://skilldrick.github.io/easy6502/
// O endereço 0xFE é reservado para geração
// de numeros aleatórios.
// IMPORTANTE: Não é comportamente nativo do 6502.
void Cpu::generateRandomIn0xFE() {
  uint8_t random = (rand() % 0xFF) + 1;
  memory.write(0xFE, random);
}

void Cpu::showCpuStatus(uint8_t index, bool showOpcodes) {
  if (showOpcodes) {
    std::cout << "| [" << count << "] " << std::dec;
    std::cout << std::hex << "OPCode: " << (int)index << " ("
                << opcodesNames[index] << ")\n";
                //| PC: 0619 | SP: 00fb | AC: 0011 | X: 0000 | Y: 0000 | SR: 
  }
  std::cout << "| PC: " << std::setfill('0') << std::hex << std::setw(4)
              << (int)PC << " | SP: " << std::setfill('0') << std::hex
              << std::setw(4) << (int)SP << " | AC: " << std::setfill('0')
              << std::hex << std::setw(4) << (int)AC
              << " | X: " << std::setfill('0') << std::hex << std::setw(4)
              << (int)X << " | Y: " << std::setfill('0') << std::hex
              << std::setw(4) << (int)Y << " | SR: " << std::bitset<8>(SR)
              << std::dec << " | CLOCK: [" << cyclesCounter << "/" << clock << "]\n";
    std::cout << "                                                           NV_BDIZC\n";
}

void Cpu::next() {
  generateRandomIn0xFE();
  uint8_t index = memory.read(PC);

  opcodeMapping[index]();
  count++;

  showCpuStatus(index, true);

  if (STOP_BRK && index == 0) {
    std::cout << "--- OPCODE BRK foi chamado. Terminando o programa. ---";
    exit(-1);
  }
}

void Cpu::reset() {
  memory.reset();
  PC = AC = X = Y = 0x00;
  SR = 0x30;
  SP = 0xFF;
}

bool isDifferentPage(uint16_t addr1, uint16_t addr2) {
  if (!((addr1 & 0xFF00) == (addr2 & 0xFF00))) {
    std::cout << "\033[33m[INFO] Add 1 to cycles if page boundary is crossed: [" << std::hex << addr1 << "] -> [" << addr2 << "]\033[0m\n\n";
  }
  return !((addr1 & 0xFF00) == (addr2 & 0xFF00));
}

// Modos de endereçamento
MemoryAccessResult Cpu::immediate() {
  uint16_t address = (PC + 1);
  return {address, 0x02, true};
}

MemoryAccessResult Cpu::zeropage() {
  uint8_t address = memory.read(PC + 1);
  return {address, 0x02, true};
}

MemoryAccessResult Cpu::zeropageX() {
  uint8_t address = memory.read(PC + 1) + X;
  return {address, 0x02, true};
}

MemoryAccessResult Cpu::zeropageY() {
  uint8_t address = memory.read(PC + 1) + Y;
  return {address, 0x02, true};
}
MemoryAccessResult Cpu::absolute() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = (msb << 8) | lsb;

  return {address, 0x03, true};
}

MemoryAccessResult Cpu::absoluteX() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = ((msb << 8) | lsb) + X;

  bool pageCrossed = isDifferentPage(((msb << 8) | lsb), address);
  
  return {address, 0x03, pageCrossed};
}

MemoryAccessResult Cpu::absoluteY() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = ((msb << 8) | lsb) + Y;

  bool pageCrossed = isDifferentPage(((msb << 8) | lsb), address);

  return {address, 0x03, pageCrossed};
}

MemoryAccessResult Cpu::indirect() {

  uint8_t msb_op = memory.read(PC + 2);
  uint8_t lsb_op = memory.read(PC + 1);
  uint16_t address_op = (msb_op << 8) | lsb_op;

  uint8_t lsb = memory.read(address_op + 0);
  uint8_t msb = memory.read(address_op + 1);

  uint16_t address = (msb << 8) | lsb;

  return {address, 0x02, true};
}

MemoryAccessResult Cpu::indirectX() {
  uint8_t zpAddress = memory.read(PC + 1) + X;
  uint8_t msb = memory.read(zpAddress + 1);
  uint8_t lsb = memory.read(zpAddress);
  uint16_t address = (msb << 8) | lsb;

  std::cout << "\033[32m[LOG] indirectX: " << "   (PC + 1):" << std::hex << (int)memory.read(PC + 1) << "\n";
  std::cout << "\033[32m[LOG] indirectX: " << "        (X):" << std::hex << (int)X << "\n";
  std::cout << "\033[32m[LOG] indirectX: " << "(zpAddress):" << std::hex << (int)zpAddress << "\n";
  std::cout << "\033[32m[LOG] indirectX: " << "  (address):" << std::hex << (int)address << "]\n\n\033[0m";
  

  return {address, 0x02, true};
}

MemoryAccessResult Cpu::indirectY() {
  uint8_t zpAddress = memory.read(PC + 1);
  uint8_t msb = memory.read(zpAddress + 1);
  uint8_t lsb = memory.read(zpAddress + 0);
  uint16_t address = ((msb << 8) | lsb) + Y;
  
  bool pageCrossed = isDifferentPage(((msb << 8) | lsb), address);

  return {address, 0x02, pageCrossed};
}

MemoryAccessResult Cpu::relative() {
  uint16_t value = memory.read(PC + 1);

  if ((value & (0x01 << 7)) == 0) {
    uint8_t offset = value;
    uint16_t address = PC + offset;
    return {address, 0x02, true};
  }

  uint8_t offset = (~value) + 1; // Compl 2
  uint16_t address = PC - offset;
  return {address, 0x02, true};
}

// -- verificadores de flags
// Negative
void Cpu::flagActivationN(uint8_t value) {
  if (value & (0x01 << 7)) {
    setFlag(Flag::N);
    return;
  }
  remFlag(Flag::N);
}

// Overflow
void Cpu::flagActivationV(uint8_t value_orig, uint8_t value_new) {
  if (((AC ^ value_orig) & (0x01 << 7)) && ((AC ^ value_new) & (0x01 << 7))) {
    setFlag(Flag::V);
    return;
  }
  remFlag(Flag::V);
}

// Break
void Cpu::flagActivationB() {}

// Decimal
void Cpu::flagActivationD() {}

// Interrupt
void Cpu::flagActivationI() {}

// Zero
void Cpu::flagActivationZ(uint8_t value) {
  if (value == 0) {
    setFlag(Flag::Z);
    return;
  }
  remFlag(Flag::Z);
}

// Carry (sum)
void Cpu::flagActivationC_ovflw(uint16_t value) {
  if (value > 0xFF) {
    setFlag(Flag::C);
    return;
  }
  remFlag(Flag::C);
}

void Cpu::flagActivationC_unflw(uint16_t value_1, uint16_t value_2) {
  if (value_2 > value_1) {
    setFlag(Flag::C);
    return;
  }
  remFlag(Flag::C);
}

// Carry (subtraction)
// Este flag é definido se não houver empréstimo durante a subtração.
void Cpu::flagActivationCMP(uint16_t value_1, uint8_t value_2) {
  // "\033[33m[INFO]  "]\033[0m\n\n";
  std::cout << "\033[32m[LOG] flagActivationCMP: " << "[" << std::hex << (int)value_1 << "/" << (int)value_2 << "]\n\n\033[0m";
  
  if (value_1 == value_2) {
    setFlag(Flag::Z);
  } else {
    remFlag(Flag::Z);
  }

  flagActivationN(value_1 - value_2);

  if (value_1 >= value_2) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }
}

// Implementações das instruções
// ADC (ADd with Carry)
void Cpu::ADC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t result = AC + value + carry;
  flagActivationC_ovflw(AC + value + carry);
  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(AC, result);
  AC = result;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// AND (bitwise AND with accumulator)
void Cpu::AND(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value & AC;
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// ASL (Arithmetic Shift Left)
void Cpu::ASL(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value << 0x01;

  (value & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);

  flagActivationN(result);
  flagActivationZ(result);
  memory.write(PC + 1, result);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// ASL (Arithmetic Shift Left) - Operações diretas no acumulador
void Cpu::ASL_AC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  uint8_t value = AC;
  uint8_t result = (AC << 0x01);

  (value & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);

  flagActivationN(result);
  flagActivationZ(result);
  AC = value;
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// BIT (test BITs)
void Cpu::BIT(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = (AC & value);

  if ((result & (0x01 << 7))) {
    setFlag(Flag::N);
  } else {
    remFlag(Flag::N);
  }
  if ((result & (0x01 << 6))) {
    setFlag(Flag::V);
  } else {
    remFlag(Flag::V);
  }
  if (result == 0x00) {
    setFlag(Flag::Z);
  } else {
    remFlag(Flag::Z);
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// Branch Instructions
// - BPL (Branch on PLus) - Desvio quando FlagN = 0
void Cpu::BPL(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (!chkFlag(Flag::N)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BMI (Branch on MInus) - Desvio quando FlagN = 1
void Cpu::BMI(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (chkFlag(Flag::N)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BVC (Branch on oVerflow Clear) - Desvio quando FlagV = 0
void Cpu::BVC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  if (!chkFlag(Flag::V)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BVS (Branch on oVerflow Set) - Desvio quando FlagV = 1
void Cpu::BVS(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (chkFlag(Flag::V)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BCC (Branch on Carry Clear) - Desvio quando FlagC = 0
void Cpu::BCC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (!chkFlag(Flag::C)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BCS (Branch on Carry Set) - Desvio quando FlagC = 1
void Cpu::BCS(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (chkFlag(Flag::C)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BNE (Branch on Not Equal) - Desvio quando FlagZ = 0
void Cpu::BNE(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (!chkFlag(Flag::Z)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// - BEQ (Branch on EQual) - Desvio quando FlagZ = 1
void Cpu::BEQ(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  if (chkFlag(Flag::Z)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// BRK (BReaK)
void Cpu::BRK(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  incrementPC(0x01);
  uint8_t PC_lsb = static_cast<uint8_t>(PC & 0xFF);
  uint8_t PC_msb = static_cast<uint8_t>(PC >> 8);

  setFlag(Flag::I);

  stackPUSH(SR);
  stackPUSH(PC_lsb);
  stackPUSH(PC_msb);

  uint8_t IRQ_msb = memory.read(0xFFFF);
  uint8_t IRQ_lsb = memory.read(0xFFFE);

  uint16_t address = (IRQ_msb << 8) | IRQ_lsb;

  PC = address;
  useCpuCicles(cycles + pageChangedCycle);
}
// CMP (CoMPare accumulator)
void Cpu::CMP(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationCMP(AC, value);

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// CPX (ComPare X register)
void Cpu::CPX(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationCMP(X, value);

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// CPY (ComPare Y register)
void Cpu::CPY(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationCMP(Y, value);

  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// DEC (DECrement memory)
void Cpu::DEC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value - 0x01;
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// EOR (bitwise Exclusive OR)
void Cpu::EOR(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = AC ^ value;
  flagActivationN(result);
  flagActivationZ(result);

  AC = result;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// Flag (Processor Status) Instructions
/// - CLC (CLear Carry)
void Cpu::CLC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::C);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - SEC (SEt Carry)
void Cpu::SEC(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::C);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - CLI (CLear Interrupt)
void Cpu::CLI(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::I);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - SEI (SEt Interrupt)
void Cpu::SEI(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::I);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - CLV (CLear oVerflow)
void Cpu::CLV(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::V);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - CLD (CLear Decimal)
void Cpu::CLD(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::D);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - SED (SEt Decimal)
void Cpu::SED(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::D);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// INC (INCrement memory)
void Cpu::INC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value + 0x01;
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// JMP (JuMP)  [ok]Teste 1
void Cpu::JMP(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  PC = response.address;
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// JSR (Jump to SubRoutine) - Salva o end. de Retorno na pilha
void Cpu::JSR(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();

  uint16_t nextOP = PC + response.size;
  uint8_t nextOP_lsb = static_cast<uint8_t>(nextOP & 0x00FF);
  uint8_t nextOP_msb = static_cast<uint8_t>(nextOP >> 0x08);

  stackPUSH(nextOP_lsb);
  stackPUSH(nextOP_msb);

  PC = response.address;
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// LDA (LoaD Accumulator)
void Cpu::LDA(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  AC = value;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// LDX (LoaD X register)ADC #$0F
void Cpu::LDX(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  X = value;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// LDY (LoaD Y register)
void Cpu::LDY(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  Y = value;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// LSR (Logical Shift Right)
void Cpu::LSR(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  (value & 0x01) ? setFlag(Flag::C) : remFlag(Flag::C);
  uint8_t result = (value >> 0x01);
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
void Cpu::LSR_AC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  (AC & 0x01) ? setFlag(Flag::C) : remFlag(Flag::C);
  uint8_t result = (AC >> 0x01);
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// NOP (No OPeration)
void Cpu::NOP(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// ORA (bitwise OR with Accumulator)
void Cpu::ORA(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = AC | value;
  flagActivationN(result);
  flagActivationZ(result);

  AC = result;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// Register Instructions
// - TAX (Transfer A to X)
void Cpu::TAX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  X = AC;
  flagActivationN(AC);
  flagActivationZ(AC);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - TXA (Transfer X to A)
void Cpu::TXA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  AC = X;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - DEX (DEcrement X)
void Cpu::DEX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  X -= 0x01;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - INX (INcrement X)
void Cpu::INX(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  X += 0x01;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - TAY (Transfer A to Y)
void Cpu::TAY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  Y = AC;
  flagActivationN(AC);
  flagActivationZ(AC);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - TYA (Transfer Y to A)
void Cpu::TYA(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  AC = Y;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - DEY (DEcrement Y)
void Cpu::DEY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  Y -= 0x01;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - INY (INcrement Y)
void Cpu::INY(MemoryAccessResult (Cpu::*AddressingMode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(AddressingMode);
  Y += 0x01;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// ROL (ROtate Left)
void Cpu::ROL(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);

  uint8_t old_carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t new_carry = (value & (0x01 << 7));

  uint8_t result = (value << 0x01) | old_carry;

  if (new_carry) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }

  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
void Cpu::ROL_AC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);

  uint8_t old_carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t new_carry = (AC & (0x01 << 7));

  uint8_t result = (AC << 0x01) | old_carry;

  if (new_carry) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }


  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// ROR (ROtate Right)
void Cpu::ROR(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  (value & 0x01) > 0 ? setFlag(Flag::C) : remFlag(Flag::C);

  uint8_t old_carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t new_carry = (value & 0x01);

  uint8_t result = (value >> 0x01) | (old_carry << 0x07);

  if (new_carry) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }

  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
void Cpu::ROR_AC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  (AC & 0x01) > 0 ? setFlag(Flag::C) : remFlag(Flag::C);

  uint8_t old_carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t new_carry = (AC & 0x01);

  uint8_t result = (AC >> 0x01) | (old_carry << 0x07);

  if (new_carry) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }

  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// RTI (ReTurn from Interrupt)
void Cpu::RTI(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  uint8_t PC_msb = stackPOP();
  uint8_t PC_lsb = stackPOP();

  SR = stackPOP();
  PC = (PC_msb << 8) | PC_lsb;
  useCpuCicles(cycles + pageChangedCycle);
}
// RTS (ReTurn from Subroutine)
void Cpu::RTS(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  uint8_t address_msb = stackPOP();
  uint8_t address_lsb = stackPOP();
  uint16_t address = (address_msb << 0x08) | address_lsb;
  PC = address;
  useCpuCicles(cycles + pageChangedCycle);
}
// SBC (SuBtract with Carry)
void Cpu::SBC(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;

  uint16_t result = AC - value - (1- carry);

  flagActivationC_unflw(AC, AC - value - (1- carry));

  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(AC, result);
  AC = result;
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
  std::cout << "\n\n\n\n";
}

// STA (STore Accumulator)
void Cpu::STA(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  memory.write(response.address, AC);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// Stack Instructions
// - TXS (Transfer X to Stack ptr)
void Cpu::TXS(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  SP = X;
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - TSX (Transfer Stack ptr to X)
void Cpu::TSX(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  X = SP;
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - PHA (PusH Accumulator)
void Cpu::PHA(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  stackPUSH(AC);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - PLA (PuLl Accumulator)
void Cpu::PLA(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  AC = stackPOP();
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - PHP (PusH Processor status)
void Cpu::PHP(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  stackPUSH(SR);
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// - PLP (PuLl Processor status)
void Cpu::PLP(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  static_cast<void>(Addressingmode);
  SR = stackPOP();
  incrementPC(0x01);
  useCpuCicles(cycles + pageChangedCycle);
}
// STX (STore X register)
void Cpu::STX(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  memory.write(response.address, X);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
// STY (STore Y register)
void Cpu::STY(MemoryAccessResult (Cpu::*Addressingmode)(), uint8_t cycles, uint8_t pageChangedCycle) {
  MemoryAccessResult response = (this->*Addressingmode)();
  memory.write(response.address, Y);
  incrementPC(response.size);
  useCpuCicles(cycles + (response.pageCrossed ? pageChangedCycle : 0));
}
