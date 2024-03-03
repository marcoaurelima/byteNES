#include "Cpu.hpp"
#include <bitset>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <ios>
#include <iostream>

Cpu::Cpu(Memory &memory) : memory(memory) { fillOpcodeMapping(); }

Cpu::~Cpu() {}

void Cpu::fillOpcodeMapping() {

  // ADC (ADd with Carry)
  opcodeMapping[0x69] = [this]() { this->ADC(&Cpu::immediate); };
  opcodeMapping[0x65] = [this]() { this->ADC(&Cpu::zeropage); };
  opcodeMapping[0x75] = [this]() { this->ADC(&Cpu::zeropageX); };
  opcodeMapping[0x6D] = [this]() { this->ADC(&Cpu::absolute); };
  opcodeMapping[0x7D] = [this]() { this->ADC(&Cpu::absoluteX); };
  opcodeMapping[0x79] = [this]() { this->ADC(&Cpu::absoluteY); };
  opcodeMapping[0x61] = [this]() { this->ADC(&Cpu::indirectX); };
  opcodeMapping[0x71] = [this]() { this->ADC(&Cpu::indirectY); };
  // AND (bitwise AND with accumulator)
  opcodeMapping[0x29] = [this]() { this->AND(&Cpu::immediate); };
  opcodeMapping[0x25] = [this]() { this->AND(&Cpu::zeropage); };
  opcodeMapping[0x35] = [this]() { this->AND(&Cpu::zeropageX); };
  opcodeMapping[0x2D] = [this]() { this->AND(&Cpu::absolute); };
  opcodeMapping[0x3D] = [this]() { this->AND(&Cpu::absoluteX); };
  opcodeMapping[0x39] = [this]() { this->AND(&Cpu::absoluteY); };
  opcodeMapping[0x21] = [this]() { this->AND(&Cpu::indirectX); };
  opcodeMapping[0x31] = [this]() { this->AND(&Cpu::indirectY); };
  // ASL (Arithmetic Shift Left)
  opcodeMapping[0x06] = [this]() { this->ASL(&Cpu::zeropage); };
  opcodeMapping[0x16] = [this]() { this->ASL(&Cpu::zeropageX); };
  opcodeMapping[0x0E] = [this]() { this->ASL(&Cpu::absolute); };
  opcodeMapping[0x1E] = [this]() { this->ASL(&Cpu::absoluteX); };
  opcodeMapping[0x0A] = [this]() { this->ASL_AC(nullptr); };
  // BIT (test BITs)
  opcodeMapping[0x24] = [this]() { this->BIT(&Cpu::zeropage); };
  opcodeMapping[0x2C] = [this]() { this->BIT(&Cpu::zeropage); };
  // Branch Instructions
  opcodeMapping[0x10] = [this]() { this->BPL(&Cpu::relative); };
  opcodeMapping[0x30] = [this]() { this->BMI(&Cpu::relative); };
  opcodeMapping[0x50] = [this]() { this->BVC(&Cpu::relative); };
  opcodeMapping[0x70] = [this]() { this->BVS(&Cpu::relative); };
  opcodeMapping[0x90] = [this]() { this->BCC(&Cpu::relative); };
  opcodeMapping[0xB0] = [this]() { this->BCS(&Cpu::relative); };
  opcodeMapping[0xD0] = [this]() { this->BNE(&Cpu::relative); };
  opcodeMapping[0xF0] = [this]() { this->BEQ(&Cpu::relative); };
  // BRK (BReaK)
  opcodeMapping[0x00] = [this]() { this->BRK(nullptr); };
  // CMP (CoMPare accumulator)
  opcodeMapping[0xC9] = [this]() { this->CMP(&Cpu::immediate); };
  opcodeMapping[0xC5] = [this]() { this->CMP(&Cpu::zeropage); };
  opcodeMapping[0xD5] = [this]() { this->CMP(&Cpu::zeropageX); };
  opcodeMapping[0xCD] = [this]() { this->CMP(&Cpu::absolute); };
  opcodeMapping[0xDD] = [this]() { this->CMP(&Cpu::absoluteX); };
  opcodeMapping[0xD9] = [this]() { this->CMP(&Cpu::absoluteY); };
  opcodeMapping[0xC1] = [this]() { this->CMP(&Cpu::indirectX); };
  opcodeMapping[0xD1] = [this]() { this->CMP(&Cpu::indirectY); };
  // CPX (ComPare X register)
  opcodeMapping[0xE0] = [this]() { this->CPX(&Cpu::immediate); };
  opcodeMapping[0xE4] = [this]() { this->CPX(&Cpu::zeropage); };
  opcodeMapping[0xEC] = [this]() { this->CPX(&Cpu::absolute); };
  // CPY (ComPare Y register)
  opcodeMapping[0xC0] = [this]() { this->CPY(&Cpu::immediate); };
  opcodeMapping[0xC4] = [this]() { this->CPY(&Cpu::zeropage); };
  opcodeMapping[0xCC] = [this]() { this->CPY(&Cpu::absolute); };
  // DEC (DECrement memory)
  opcodeMapping[0xC6] = [this]() { this->DEC(&Cpu::zeropage); };
  opcodeMapping[0xD6] = [this]() { this->DEC(&Cpu::zeropageX); };
  opcodeMapping[0xCE] = [this]() { this->DEC(&Cpu::absolute); };
  opcodeMapping[0xDE] = [this]() { this->DEC(&Cpu::absoluteX); };
  // EOR (bitwise Exclusive OR)
  opcodeMapping[0x49] = [this]() { this->EOR(&Cpu::immediate); };
  opcodeMapping[0x45] = [this]() { this->EOR(&Cpu::zeropage); };
  opcodeMapping[0x55] = [this]() { this->EOR(&Cpu::zeropageX); };
  opcodeMapping[0x4D] = [this]() { this->EOR(&Cpu::absolute); };
  opcodeMapping[0x5D] = [this]() { this->EOR(&Cpu::absoluteX); };
  opcodeMapping[0x59] = [this]() { this->EOR(&Cpu::absoluteY); };
  opcodeMapping[0x41] = [this]() { this->EOR(&Cpu::indirectX); };
  opcodeMapping[0x51] = [this]() { this->EOR(&Cpu::indirectY); };
  // Flag (Processor Status) Instructions
  opcodeMapping[0x18] = [this]() { this->CLC(nullptr); };
  opcodeMapping[0x38] = [this]() { this->SEC(nullptr); };
  opcodeMapping[0x58] = [this]() { this->CLI(nullptr); };
  opcodeMapping[0x78] = [this]() { this->SEI(nullptr); };
  opcodeMapping[0xB8] = [this]() { this->CLV(nullptr); };
  opcodeMapping[0xD8] = [this]() { this->CLD(nullptr); };
  opcodeMapping[0xF8] = [this]() { this->SED(nullptr); };
  // INC (INCrement memory)
  opcodeMapping[0xE6] = [this]() { this->INC(&Cpu::zeropage); };
  opcodeMapping[0xF6] = [this]() { this->INC(&Cpu::zeropageX); };
  opcodeMapping[0xEE] = [this]() { this->INC(&Cpu::absolute); };
  opcodeMapping[0xFE] = [this]() { this->INC(&Cpu::absoluteX); };
  // JMP (JuMP)
  opcodeMapping[0x4C] = [this]() { this->JMP(&Cpu::absolute); };
  opcodeMapping[0x6C] = [this]() { this->JMP(&Cpu::indirect); };
  // JSR (Jump to SubRoutine)
  opcodeMapping[0x20] = [this]() { this->JSR(&Cpu::absolute); };
  // LDA (LoaD Accumulator)
  opcodeMapping[0xA9] = [this]() { this->LDA(&Cpu::immediate); };
  opcodeMapping[0xA5] = [this]() { this->LDA(&Cpu::zeropage); };
  opcodeMapping[0xB5] = [this]() { this->LDA(&Cpu::zeropageX); };
  opcodeMapping[0xAD] = [this]() { this->LDA(&Cpu::absolute); };
  opcodeMapping[0xBD] = [this]() { this->LDA(&Cpu::absoluteX); };
  opcodeMapping[0xB9] = [this]() { this->LDA(&Cpu::absoluteY); };
  opcodeMapping[0xA1] = [this]() { this->LDA(&Cpu::indirectX); };
  opcodeMapping[0xB1] = [this]() { this->LDA(&Cpu::indirectY); };
  // LDX (LoaD X register)
  opcodeMapping[0xA2] = [this]() { this->LDX(&Cpu::immediate); };
  opcodeMapping[0xA6] = [this]() { this->LDX(&Cpu::zeropage); };
  opcodeMapping[0xB6] = [this]() { this->LDX(&Cpu::zeropageY); };
  opcodeMapping[0xAE] = [this]() { this->LDX(&Cpu::absolute); };
  opcodeMapping[0xBE] = [this]() { this->LDX(&Cpu::absoluteY); };
  // LDY (LoaD Y register)
  opcodeMapping[0xA0] = [this]() { this->LDY(&Cpu::immediate); };
  opcodeMapping[0xA4] = [this]() { this->LDY(&Cpu::zeropage); };
  opcodeMapping[0xB4] = [this]() { this->LDY(&Cpu::zeropageX); };
  opcodeMapping[0xAC] = [this]() { this->LDY(&Cpu::absolute); };
  opcodeMapping[0xBC] = [this]() { this->LDY(&Cpu::absoluteX); };
  // LSR (Logical Shift Right)
  opcodeMapping[0x46] = [this]() { this->LSR(&Cpu::zeropage); };
  opcodeMapping[0x56] = [this]() { this->LSR(&Cpu::zeropageX); };
  opcodeMapping[0x4E] = [this]() { this->LSR(&Cpu::absolute); };
  opcodeMapping[0x5E] = [this]() { this->LSR(&Cpu::absoluteX); };
  opcodeMapping[0x4A] = [this]() { this->LSR_AC(nullptr); };
  // NOP (No OPeration)
  opcodeMapping[0xEA] = [this]() { this->NOP(nullptr); };
  // ORA (bitwise OR with Accumulator)
  opcodeMapping[0x09] = [this]() { this->ORA(&Cpu::immediate); };
  opcodeMapping[0x05] = [this]() { this->ORA(&Cpu::zeropage); };
  opcodeMapping[0x15] = [this]() { this->ORA(&Cpu::zeropageX); };
  opcodeMapping[0x0D] = [this]() { this->ORA(&Cpu::absolute); };
  opcodeMapping[0x1D] = [this]() { this->ORA(&Cpu::absoluteX); };
  opcodeMapping[0x19] = [this]() { this->ORA(&Cpu::absoluteY); };
  opcodeMapping[0x01] = [this]() { this->ORA(&Cpu::indirectX); };
  opcodeMapping[0x11] = [this]() { this->ORA(&Cpu::indirectY); };
  // Register Instructions
  opcodeMapping[0xAA] = [this]() { this->TAX(nullptr); };
  opcodeMapping[0x8A] = [this]() { this->TXA(nullptr); };
  opcodeMapping[0xCA] = [this]() { this->DEX(nullptr); };
  opcodeMapping[0xE8] = [this]() { this->INX(nullptr); };
  opcodeMapping[0xA8] = [this]() { this->TAY(nullptr); };
  opcodeMapping[0x98] = [this]() { this->TYA(nullptr); };
  opcodeMapping[0x88] = [this]() { this->DEY(nullptr); };
  opcodeMapping[0xC8] = [this]() { this->INY(nullptr); };
  // ROL (ROtate Left)
  opcodeMapping[0x26] = [this]() { this->ROL(&Cpu::zeropage); };
  opcodeMapping[0x36] = [this]() { this->ROL(&Cpu::zeropageX); };
  opcodeMapping[0x2E] = [this]() { this->ROL(&Cpu::absolute); };
  opcodeMapping[0x3E] = [this]() { this->ROL(&Cpu::absoluteX); };
  opcodeMapping[0x2A] = [this]() { this->ROL_AC(nullptr); };
  // ROR (ROtate Right)
  opcodeMapping[0x66] = [this]() { this->ROR(&Cpu::zeropage); };
  opcodeMapping[0x76] = [this]() { this->ROR(&Cpu::zeropageX); };
  opcodeMapping[0x6E] = [this]() { this->ROR(&Cpu::absolute); };
  opcodeMapping[0x7E] = [this]() { this->ROR(&Cpu::absoluteX); };
  opcodeMapping[0x6A] = [this]() { this->ROR_AC(nullptr); };
  // RTI (ReTurn from Interrupt)
  opcodeMapping[0x40] = [this]() { this->RTI(nullptr); };
  // RTS (ReTurn from Subroutine)
  opcodeMapping[0x60] = [this]() { this->RTS(nullptr); };
  // SBC (SuBtract with Carry)
  opcodeMapping[0xE9] = [this]() { this->SBC(&Cpu::immediate); };
  opcodeMapping[0xE5] = [this]() { this->SBC(&Cpu::zeropage); };
  opcodeMapping[0xF5] = [this]() { this->SBC(&Cpu::zeropageX); };
  opcodeMapping[0xED] = [this]() { this->SBC(&Cpu::absolute); };
  opcodeMapping[0xFD] = [this]() { this->SBC(&Cpu::absoluteX); };
  opcodeMapping[0xF9] = [this]() { this->SBC(&Cpu::absoluteY); };
  opcodeMapping[0xE1] = [this]() { this->SBC(&Cpu::indirectX); };
  opcodeMapping[0xF1] = [this]() { this->SBC(&Cpu::indirectY); };
  // STA (STore Accumulator)
  opcodeMapping[0x85] = [this]() { this->STA(&Cpu::zeropage); };
  opcodeMapping[0x95] = [this]() { this->STA(&Cpu::zeropageX); };
  opcodeMapping[0x8D] = [this]() { this->STA(&Cpu::absolute); };
  opcodeMapping[0x9D] = [this]() { this->STA(&Cpu::absoluteX); };
  opcodeMapping[0x99] = [this]() { this->STA(&Cpu::absoluteY); };
  opcodeMapping[0x81] = [this]() { this->STA(&Cpu::indirectX); };
  opcodeMapping[0x91] = [this]() { this->STA(&Cpu::indirectY); };
  // Stack Instructions
  opcodeMapping[0x9A] = [this]() { this->TXS(nullptr); };
  opcodeMapping[0xBA] = [this]() { this->TSX(nullptr); };
  opcodeMapping[0x48] = [this]() { this->PHA(nullptr); };
  opcodeMapping[0x68] = [this]() { this->PLA(nullptr); };
  opcodeMapping[0x08] = [this]() { this->PHP(nullptr); };
  opcodeMapping[0x28] = [this]() { this->PLP(nullptr); };
  // STX (STore X register)
  opcodeMapping[0x86] = [this]() { this->STX(&Cpu::zeropage); };
  opcodeMapping[0x96] = [this]() { this->STX(&Cpu::zeropageY); };
  opcodeMapping[0x8E] = [this]() { this->STX(&Cpu::absolute); };
  // STY (STore Y register)
  opcodeMapping[0x84] = [this]() { this->STY(&Cpu::zeropage); };
  opcodeMapping[0x94] = [this]() { this->STY(&Cpu::zeropageY); };
  opcodeMapping[0x8C] = [this]() { this->STY(&Cpu::absolute); };

  srand(time(NULL));
}

void Cpu::setAsmAddress(uint16_t address) {
  asmAddress = address;
  PC = address;
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

void Cpu::next() {
  generateRandomIn0xFE();
  uint8_t index = memory.read(PC);

  bool ENABLE_LOGS = true;

  if (ENABLE_LOGS) {
    std::cout
        << "--[" << std::dec << count + 1
        << "]--------------------------------------------------------------\n";
    std::cout << "| PC: " << std::setfill('0') << std::hex << std::setw(4)
              << (int)PC << " | SP: " << std::setfill('0') << std::hex
              << std::setw(4) << (int)SP << " | AC: " << std::setfill('0')
              << std::hex << std::setw(4) << (int)AC
              << " | X: " << std::setfill('0') << std::hex << std::setw(4)
              << (int)X << " | Y: " << std::setfill('0') << std::hex
              << std::setw(4) << (int)Y << " | SR: " << std::bitset<8>(SR)
              << "\n";

    std::cout << std::hex << "| OPCode: " << (int)index << " ("
              << opcodesNames[index] << ")\n";
  }

  opcodeMapping[index]();
  count++;

  if (ENABLE_LOGS) {
    // std::cout << std::hex << "| PC: " << (int)PC << " | SP: " << (int)SP
    //           << " | AC: " << (int)AC << " | X: " << (int)X
    //           << " | Y: " << (int)Y << " | SR: " << std::bitset<8>(SR)
    //           << "\n\n";
    // std::cout << "-------------------------------------------------------\n";
    std::cout << "| PC: " << std::setfill('0') << std::hex << std::setw(4)
              << (int)PC << " | SP: " << std::setfill('0') << std::hex
              << std::setw(4) << (int)SP << " | AC: " << std::setfill('0')
              << std::hex << std::setw(4) << (int)AC
              << " | X: " << std::setfill('0') << std::hex << std::setw(4)
              << (int)X << " | Y: " << std::setfill('0') << std::hex
              << std::setw(4) << (int)Y << " | SR: " << std::bitset<8>(SR)
              << "\n\n";
  }

  if (STOP_BRK && index == 0) {
    std::cout << "--- OPCODE BRK foi chamado. Terminando o programa. ---";
    exit(-1);
  }
}

void Cpu::reset() {
  memory.reset();
  PC = AC = X = Y = 0X00;
  SR = 0x30;
  SP = 0xFF;
}

// Modos de endereçamento
AMResponse Cpu::immediate() {
  uint16_t address = (PC + 1);

  return {address, 0x01};
}

AMResponse Cpu::zeropage() {
  uint8_t address = memory.read(PC + 1);
  return {address, 0x01};
}

AMResponse Cpu::zeropageX() {
  uint8_t address = memory.read(PC + X);
  return {address, 0x01};
}

AMResponse Cpu::zeropageY() {
  uint8_t address = memory.read(PC + Y);
  return {address, 0x01};
}
AMResponse Cpu::absolute() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = (msb << 8) | lsb;

  return {address, 0x02};
}

AMResponse Cpu::absoluteX() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = ((msb << 8) | lsb) + X;

  return {address, 0x02};
}

AMResponse Cpu::absoluteY() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = ((msb << 8) | lsb) + Y;

  return {address, 0x02};
}

AMResponse Cpu::indirect() {

  uint8_t msb_op = memory.read(PC + 2);
  uint8_t lsb_op = memory.read(PC + 1);
  uint16_t address_op = (msb_op << 8) | lsb_op;

  uint8_t lsb = memory.read(address_op + 0);
  uint8_t msb = memory.read(address_op + 1);

  uint16_t address = (msb << 8) | lsb;

  return {address, 0x01};
}

AMResponse Cpu::indirectX() {
  // uint8_t msb = memory.read((PC + 1) + X);
  // uint8_t lsb = memory.read((PC + 1) + X + 1);
  // uint16_t address = (msb << 8) | lsb;
  //
  uint8_t zpAddress = memory.read(PC + 1);
  uint8_t msb = memory.read(zpAddress + X + 1);
  uint8_t lsb = memory.read(zpAddress + X);
  uint16_t address = (msb << 8) | lsb;

  // std::cout << "indirectX ZPADD | X: " << (int)zpAddress << " | " << (int)X
  // << "\n"; std::cout << "indirectX msb: " << (int)msb << "\n"; std::cout <<
  // "indirectX lsb:" << (int)lsb << "\n"; std::cout << "indirectX: " <<
  // (int)address << "\n";

  return {address, 0x01};
}

AMResponse Cpu::indirectY() {
  // uint8_t msb = memory.read(PC + 2);
  // uint8_t lsb = memory.read(PC + 1);
  // uint16_t address = (msb << 8) | lsb;
  //
  // return {address, 0x01};

  uint8_t zpAddress = memory.read(PC + 1);
  uint8_t msb = memory.read(zpAddress + 1);
  uint8_t lsb = memory.read(zpAddress);
  uint16_t address = ((msb << 8) | lsb) + Y;

  // std::cout << "indirectY ZPADD | X: " << (int)zpAddress << " | " << (int)X
  // << "\n"; std::cout << "indirectY msb: " << (int)msb << "\n"; std::cout <<
  // "indirectY lsb:" << (int)lsb << "\n"; std::cout << "indirectY: " <<
  // (int)address << "\n";

  return {address, 0x01};
}

// AMResponse Cpu::relative() {
//   uint16_t value = memory.read(PC + 1);
//   uint8_t offset = ~(0x01 << 7) & value;
//
//   if ((value & (0x01 << 7)) == 0) {
//     uint16_t address = PC + offset;
//     return {address, 0x01};
//   }
//
//   uint16_t address = PC - offset;
//   return {address, 0x01};
// }
AMResponse Cpu::relative() {
  uint16_t value = memory.read(PC + 1);

  if ((value & (0x01 << 7)) == 0) {
    uint8_t offset = value;
    uint16_t address = PC + offset;
    return {address, 0x02};
  }

  uint8_t offset = (~value) + 1; // Compl 2
  uint16_t address = PC - offset;
  return {address, 0x02};
}

// -- verificadores de flags
// Negative
void Cpu::flagActivationN(uint8_t value) {
  // std::cout << "flagActivationN: " << (int)value << " : "
            // << std::bitset<8>(value) << std::endl;
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

  // std::cout << "flagActivationCMP: " << (int)value_1 << " | " << (int)value_2
            // << "\n";

  // if (value_1 < value_2) {
  //   remFlag(Flag::Z);
  //   remFlag(Flag::C);
  //   flagActivationN(value_1 - value_2);
  //   return;
  // }
  //
  // if (value_1 == value_2) {
  //   setFlag(Flag::Z);
  //   setFlag(Flag::C);
  //   remFlag(Flag::N);
  //   return;
  // }
  //
  // if (value_1 > value_2) {
  //   remFlag(Flag::Z);
  //   setFlag(Flag::C);
  //   flagActivationN(value_1 - value_2);
  //   return;
  // }

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
void Cpu::ADC(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t result = AC + value + carry;
  flagActivationC_ovflw(AC + value + carry);
  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(AC, result);
  AC = result;
  incrementPC(response.size + 0x01);
}
// AND (bitwise AND with accumulator)
void Cpu::AND(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value & AC;
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(response.size + 0x01);
}
// ASL (Arithmetic Shift Left)
void Cpu::ASL(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value << 0x01;

  (value & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);

  flagActivationN(result);
  flagActivationZ(result);
  memory.write(PC + 1, result);
  incrementPC(response.size + 0x01);
}
// ASL (Arithmetic Shift Left) - Operações diretas no acumulador
void Cpu::ASL_AC(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  uint8_t value = AC;
  uint8_t result = (AC << 0x01);

  (value & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);

  flagActivationN(result);
  flagActivationZ(result);
  AC = value;
  incrementPC(0x01);
}
// BIT (test BITs)
void Cpu::BIT(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
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

  incrementPC(response.size + 0x01);
}
// Branch Instructions
// - BPL (Branch on PLus) - Desvio quando FlagN = 0
void Cpu::BPL(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (!chkFlag(Flag::N)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BMI (Branch on MInus) - Desvio quando FlagN = 1
void Cpu::BMI(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (chkFlag(Flag::N)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BVC (Branch on oVerflow Clear) - Desvio quando FlagV = 0
void Cpu::BVC(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  // std::cout << "BVC addr: " << (int)response.address << "  size: " <<
  // (int)response.size << "\n";
  if (!chkFlag(Flag::V)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BVS (Branch on oVerflow Set) - Desvio quando FlagV = 1
void Cpu::BVS(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (chkFlag(Flag::V)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BCC (Branch on Carry Clear) - Desvio quando FlagC = 0
void Cpu::BCC(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (!chkFlag(Flag::C)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BCS (Branch on Carry Set) - Desvio quando FlagC = 1
void Cpu::BCS(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (chkFlag(Flag::C)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BNE (Branch on Not Equal) - Desvio quando FlagZ = 0
void Cpu::BNE(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (!chkFlag(Flag::Z)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// - BEQ (Branch on EQual) - Desvio quando FlagZ = 1
void Cpu::BEQ(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  if (chkFlag(Flag::Z)) {
    PC = response.address + response.size;
    return;
  }

  incrementPC(response.size);
}
// BRK (BReaK)
void Cpu::BRK(AMResponse (Cpu::*Addressingmode)()) {
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
}
// CMP (CoMPare accumulator)
void Cpu::CMP(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  // uint8_t result = AC - value;
  flagActivationCMP(AC, value);
  // flagActivationN(result);
  // flagActivationZ(result);
  // AC = result;
  incrementPC(response.size + 0x01);
}
// CPX (ComPare X register)
void Cpu::CPX(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  // uint8_t result = X - value;
  flagActivationCMP(X, value);
  // flagActivationN(result);
  // flagActivationZ(result);
  // AC = result;
  incrementPC(response.size + 0x01);
}
// CPY (ComPare Y register)
void Cpu::CPY(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  // uint8_t result = Y - value;
  flagActivationCMP(Y, value);
  // flagActivationN(result);
  // flagActivationZ(result);
  // AC = result;
  incrementPC(response.size + 0x01);
}
// DEC (DECrement memory)
void Cpu::DEC(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value - 0x01;
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size + 0x01);
}
// EOR (bitwise Exclusive OR)
void Cpu::EOR(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = AC ^ value;
  flagActivationN(result);
  flagActivationZ(result);
  // memory.write(response.address, result);
  AC = result;
  incrementPC(response.size + 0x01);
}
// Flag (Processor Status) Instructions
/// - CLC (CLear Carry)
void Cpu::CLC(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::C);
  incrementPC(0x01);
}
// - SEC (SEt Carry)
void Cpu::SEC(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::C);
  incrementPC(0x01);
}
// - CLI (CLear Interrupt)
void Cpu::CLI(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::I);
  incrementPC(0x01);
}
// - SEI (SEt Interrupt)
void Cpu::SEI(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::I);
  incrementPC(0x01);
}
// - CLV (CLear oVerflow)
void Cpu::CLV(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::V);
  incrementPC(0x01);
}
// - CLD (CLear Decimal)
void Cpu::CLD(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::D);
  incrementPC(0x01);
}
// - SED (SEt Decimal)
void Cpu::SED(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::D);
  incrementPC(0x01);
}
// INC (INCrement memory)
void Cpu::INC(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = value + 0x01;
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size + 0x01);
}
// JMP (JuMP)  [ok]Teste 1
void Cpu::JMP(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  // std::cout << "JMP - addr: " << (int)response.address << "\n";
  PC = response.address;
  // uint8_t value = memory.read(response.address);
  // PC = value;
}
// JSR (Jump to SubRoutine) - Salva o end. de Retorno na pilha
void Cpu::JSR(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();

  uint16_t nextOP = PC + response.size + 0x01;
  uint8_t nextOP_lsb = static_cast<uint8_t>(nextOP & 0x00FF);
  uint8_t nextOP_msb = static_cast<uint8_t>(nextOP >> 0x08);
  // std::cout << "nextOP: " << std::hex << (int)nextOP << "\n";
  // std::cout << "nextOP_lsb: " << std::hex << (int)nextOP_lsb << "\n";
  // std::cout << "nextOP_msb: " << std::hex << (int)nextOP_msb << "\n";
  stackPUSH(nextOP_lsb);
  stackPUSH(nextOP_msb);

  PC = response.address;
}
// LDA (LoaD Accumulator)
void Cpu::LDA(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  AC = value;
  incrementPC(response.size + 0x01);
}
// LDX (LoaD X register)ADC #$0F
void Cpu::LDX(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  X = value;
  incrementPC(response.size + 0x01);
}
// LDY (LoaD Y register)
void Cpu::LDY(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  Y = value;
  incrementPC(response.size + 0x01);
}
// LSR (Logical Shift Right)
void Cpu::LSR(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  (value & 0x01) ? setFlag(Flag::C) : remFlag(Flag::C);
  uint8_t result = (value >> 0x01);
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size + 0x01);
}
void Cpu::LSR_AC(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  (AC & 0x01) ? setFlag(Flag::C) : remFlag(Flag::C);
  uint8_t result = (AC >> 0x01);
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
}
// NOP (No OPeration)
void Cpu::NOP(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  incrementPC(0x01);
}
// ORA (bitwise OR with Accumulator)
void Cpu::ORA(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t result = AC | value;
  flagActivationN(result);
  flagActivationZ(result);
  // memory.write(response.address, result);
  AC = result;
  incrementPC(response.size + 0x01);
}
// Register Instructions
// - TAX (Transfer A to X)
void Cpu::TAX(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  X = AC;
  flagActivationN(AC);
  flagActivationZ(AC);
  incrementPC(0x01);
}
// - TXA (Transfer X to A)
void Cpu::TXA(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  AC = X;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
}
// - DEX (DEcrement X)
void Cpu::DEX(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  X -= 0x01;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
}
// - INX (INcrement X)
void Cpu::INX(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  X += 0x01;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
}
// - TAY (Transfer A to Y)
void Cpu::TAY(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  Y = AC;
  flagActivationN(AC);
  flagActivationZ(AC);
  incrementPC(0x01);
}
// - TYA (Transfer Y to A)
void Cpu::TYA(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  AC = Y;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
}
// - DEY (DEcrement Y)
void Cpu::DEY(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  Y -= 0x01;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
}
// - INY (INcrement Y)
void Cpu::INY(AMResponse (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  Y += 0x01;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
}
// ROL (ROtate Left)
void Cpu::ROL(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);

  uint8_t old_carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t new_carry = (value & (0x01 << 7));

  uint8_t result = (value << 0x01) | old_carry;

  if (new_carry) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }
  // (result & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size + 0x01);
}
void Cpu::ROL_AC(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);

  uint8_t old_carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t new_carry = (AC & (0x01 << 7));

  uint8_t result = (AC << 0x01) | old_carry;

  if (new_carry) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }

  // (result & (0x01 << 7)) > 0 ? setFlag(Flag::C) : remFlag(Flag::C);
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
}
// ROR (ROtate Right)
void Cpu::ROR(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
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
  incrementPC(response.size + 0x01);
}
void Cpu::ROR_AC(AMResponse (Cpu::*Addressingmode)()) {
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
}
// RTI (ReTurn from Interrupt)
void Cpu::RTI(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  uint8_t PC_msb = stackPOP();
  uint8_t PC_lsb = stackPOP();

  SR = stackPOP();
  PC = (PC_msb << 8) | PC_lsb;
}
// RTS (ReTurn from Subroutine)
void Cpu::RTS(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  uint8_t address_msb = stackPOP();
  uint8_t address_lsb = stackPOP();
  uint16_t address = (address_msb << 0x08) | address_lsb;
  PC = address;
}
// SBC (SuBtract with Carry)
void Cpu::SBC(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  uint8_t value = memory.read(response.address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  // uint8_t result = AC - value + carry;
  uint8_t result = AC - value + carry;
  // ATENÇÃO
  // -flagActivationC_Sub---------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // flagActivationC_Sub(result, value);
  // if ((AC - value + carry) >= 0x0100) {
  //   setFlag(Flag::C); // Ativa a flag de carry se houver empréstimo
  // } else {
  //   remFlag(Flag::C);
  // }

  flagActivationC_unflw(AC, AC - value + carry);

  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(AC, result);
  AC = result;
  incrementPC(response.size + 0x01);
}

// STA (STore Accumulator)
void Cpu::STA(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  memory.write(response.address, AC);
  incrementPC(response.size + 0x01);
}
// Stack Instructions
// - TXS (Transfer X to Stack ptr)
void Cpu::TXS(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  SP = X;
  incrementPC(0x01);
}
// - TSX (Transfer Stack ptr to X)
void Cpu::TSX(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  X = SP;
  incrementPC(0x01);
}
// - PHA (PusH Accumulator)
void Cpu::PHA(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  stackPUSH(AC);
  incrementPC(0x01);
}
// - PLA (PuLl Accumulator)
void Cpu::PLA(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  AC = stackPOP();
  incrementPC(0x01);
}
// - PHP (PusH Processor status)
void Cpu::PHP(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  stackPUSH(SR);
  incrementPC(0x01);
}
// - PLP (PuLl Processor status)
void Cpu::PLP(AMResponse (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  SR = stackPOP();
  incrementPC(0x01);
}
// STX (STore X register)
void Cpu::STX(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  memory.write(response.address, X);
  incrementPC(response.size + 0x01);
}
// STY (STore Y register)
void Cpu::STY(AMResponse (Cpu::*Addressingmode)()) {
  AMResponse response = (this->*Addressingmode)();
  memory.write(response.address, Y);
  incrementPC(response.size + 0x01);
}
