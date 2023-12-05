#include "Cpu.hpp"
#include <cstdint>
#include <sys/types.h>

Cpu::Cpu(Memory &memory, uint16_t PC, uint8_t SP, uint8_t AC, uint8_t X,
         uint8_t Y, uint8_t SR)
    : memory(memory), PC(PC), SP(SP), AC(AC), X(X), Y(Y), SR(SR) {
  fillOpcodeMapping();
}

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
  // BRK (BReaK)
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
  // CPY (ComPare Y register)
  // DEC (DECrement memory)
  opcodeMapping[0xC6] = [this]() { this->DEC(&Cpu::zeropage); };
  opcodeMapping[0xD6] = [this]() { this->DEC(&Cpu::zeropageX); };
  opcodeMapping[0xCE] = [this]() { this->DEC(&Cpu::absolute); };
  opcodeMapping[0xDE] = [this]() { this->DEC(&Cpu::absoluteX); };
  // EOR (bitwise Exclusive OR)
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
  // JSR (Jump to SubRoutine)
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
  // NOP (No OPeration)
  opcodeMapping[0xEA] = [this]() { this->NOP(&Cpu::absoluteX); };
  // ORA (bitwise OR with Accumulator)
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
  // ROR (ROtate Right)
  // RTI (ReTurn from Interrupt)
  // RTS (ReTurn from Subroutine)
  // SBC (SuBtract with Carry)
  // STA (STore Accumulator)
  // Stack Instructions
  // STX (STore X register)
  opcodeMapping[0x86] = [this]() { this->STX(&Cpu::zeropage); };
  opcodeMapping[0x96] = [this]() { this->STX(&Cpu::zeropageY); };
  opcodeMapping[0x8E] = [this]() { this->STX(&Cpu::absolute); };
  // STY (STore Y register)
  opcodeMapping[0x84] = [this]() { this->STY(&Cpu::zeropage); };
  opcodeMapping[0x94] = [this]() { this->STY(&Cpu::zeropageY); };
  opcodeMapping[0x8C] = [this]() { this->STY(&Cpu::absolute); };
}

Memory &Cpu::getMemory() { return memory; }
uint16_t Cpu::getPC() { return PC; }
uint8_t Cpu::getSP() { return SP; }
uint8_t Cpu::getAC() { return AC; }
uint8_t Cpu::getX() { return X; }
uint8_t Cpu::getY() { return Y; }
uint8_t Cpu::getSR() { return SR; }

void Cpu::setFlag(Flag flag) { SR = SR | static_cast<uint8_t>(flag); }
void Cpu::remFlag(Flag flag) { SR = SR & ~(static_cast<uint8_t>(flag)); }
bool Cpu::chkFlag(Flag flag) { return (SR & static_cast<uint8_t>(flag)) != 0; }

void Cpu::incrementPC(uint16_t value) { PC += value; }

void Cpu::next() {
  uint8_t index = memory.read(PC);
  opcodeMapping[index]();
}

void Cpu::reset() {
  memory.reset();
  PC = SP = AC = X = Y = SR = 0X00;
}

// Modos de endereçamento
uint16_t Cpu::immediate() {
  uint8_t address = (PC + 1);
  incrementPC(0x01);
  return address;
}

uint16_t Cpu::zeropage() {
  uint8_t address = memory.read(PC + 1);
  incrementPC(0x01);
  return address;
}

uint16_t Cpu::zeropageX() {
  uint8_t address = memory.read(PC + 1);
  incrementPC(0x01);
  return (address + X);
}

uint16_t Cpu::zeropageY() {
  uint8_t address = memory.read(PC + 1);
  incrementPC(0x01);
  return (address + Y);
}
uint16_t Cpu::absolute() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = (msb << 8) | lsb;

  incrementPC(0x02);
  return address;
}

uint16_t Cpu::absoluteX() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = (msb << 8) | lsb;

  incrementPC(0x02);
  return (address + X);
}

uint16_t Cpu::absoluteY() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = (msb << 8) | lsb;

  incrementPC(0x02);
  return (address + Y);
}

uint16_t Cpu::indirectX() {
  uint8_t msb = memory.read(PC + X + 2);
  uint8_t lsb = memory.read(PC + X + 1);
  uint16_t address = (msb << 8) | lsb;

  incrementPC(0x01);
  return address;
}

uint16_t Cpu::indirectY() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = (msb << 8) | lsb;

  incrementPC(0x01);
  return (address + Y);
}

// -- verificadores de flags
// Nwgative
void Cpu::flagActivationN(uint8_t value) {
  if (value & (0x01 << 7)) {
    setFlag(Flag::N);
  }
}

// Overflow
void Cpu::flagActivationV(uint8_t value_orig, uint8_t value_new) {
  if (((AC ^ value_orig) & (0x01 << 7)) && ((AC ^ value_new) & (0x01 << 7))) {
    setFlag(Flag::V);
  }
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
  }
}

// Carry (sum)
void Cpu::flagActivationC_Sum(uint16_t value) {
  if (value > 0xFF) {
    setFlag(Flag::C);
  }
}

// Carry (subtraction)
// Este flag é definido se não houver empréstimo durante a subtração.
void Cpu::flagActivationC_Sub(uint16_t result, uint8_t value) {
  if (result >= value) {
    setFlag(Flag::C);
  } else {
    remFlag(Flag::C);
  }
}

// Implementações das instruções
// ADC (ADd with Carry)
void Cpu::ADC(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t result = AC + value + carry;
  flagActivationC_Sum(AC + value + carry);
  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(value, result);
  AC = result;
  incrementPC(0x01);
}
// AND (bitwise AND with accumulator)
void Cpu::AND(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t result = value & AC;
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
}
// ASL (Arithmetic Shift Left)
void Cpu::ASL(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t result = value << 0x01;
  flagActivationN(result);
  flagActivationC_Sum(value << 0x01);
  flagActivationZ(result);
  memory.write(PC + 1, result);
  incrementPC(0x01);
}
// ASL (Arithmetic Shift Left) - Operações diretas no acumulador
void Cpu::ASL_AC(uint16_t (Cpu::*Addressingmode)()) {
  static_cast<void>(Addressingmode);
  uint8_t value = AC;
  uint8_t result = (AC << 0x01);
  flagActivationN(result);
  flagActivationC_Sum(AC << 0x01);
  flagActivationZ(result);
  AC = value;
  incrementPC(0x01);
}
// BIT (test BITs)
void Cpu::BIT(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t result = (AC & value);

  if ((result & (0x01 << 7)) > 0x00) {
    setFlag(Flag::N);
  }
  if ((result & (0x01 << 6)) > 0x00) {
    setFlag(Flag::V);
  }
  if (result == 0x00) {
    setFlag(Flag::Z);
  }
}
// Branch Instructions
// BRK (BReaK)
// CMP (CoMPare accumulator)
void Cpu::CMP(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t result = AC - value;
  flagActivationC_Sub(result, value);
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
}
// CPX (ComPare X register)
// CPY (ComPare Y register)
// DEC (DECrement memory)
void Cpu::DEC(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  flagActivationN(value);
  flagActivationZ(value);
  memory.write(address, value - 1);
  incrementPC(0x01);
}
// EOR (bitwise Exclusive OR)
// Flag (Processor Status) Instructions
/// - CLC (CLear Carry)
void Cpu::CLC(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::C);
  incrementPC(0x01);
}
// - SEC (SEt Carry)
void Cpu::SEC(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::C);
  incrementPC(0x01);
}
// - CLI (CLear Interrupt)
void Cpu::CLI(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::I);
  incrementPC(0x01);
}
// - SEI (SEt Interrupt)
void Cpu::SEI(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::I);
  incrementPC(0x01);
}
// - CLV (CLear oVerflow)
void Cpu::CLV(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::V);
  incrementPC(0x01);
}
// - CLD (CLear Decimal)
void Cpu::CLD(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  remFlag(Flag::D);
  incrementPC(0x01);
}
// - SED (SEt Decimal)
void Cpu::SED(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  setFlag(Flag::D);
  incrementPC(0x01);
}
// INC (INCrement memory)
void Cpu::INC(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  flagActivationN(value);
  flagActivationZ(value);
  memory.write(address, value + 1);
  incrementPC(0x01);
}
// JMP (JuMP)
// JSR (Jump to SubRoutine)
// LDA (LoaD Accumulator)
void Cpu::LDA(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  flagActivationN(value);
  flagActivationZ(value);
  AC = value;
  incrementPC(0x01);
}
// LDX (LoaD X register)
void Cpu::LDX(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  flagActivationN(value);
  flagActivationZ(value);
  X = value;
  incrementPC(0x01);
}
// LDY (LoaD Y register)
void Cpu::LDY(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  flagActivationN(value);
  flagActivationZ(value);
  Y = value;
  incrementPC(0x01);
}
// LSR (Logical Shift Right)
// NOP (No OPeration)
void Cpu::NOP(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  incrementPC(0x01);
}
// ORA (bitwise OR with Accumulator)
// Register Instructions
// - TAX (Transfer A to X)
void Cpu::TAX(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  X = AC;
  incrementPC(0x01);
}
// - TXA (Transfer X to A)
void Cpu::TXA(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  AC = X;
  incrementPC(0x01);
}
// - DEX (DEcrement X)
void Cpu::DEX(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  X -= 0x01;
  incrementPC(0x01);
}
// - INX (INcrement X)
void Cpu::INX(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  X += 0x01;
  incrementPC(0x01);
}
// - TAY (Transfer A to Y)
void Cpu::TAY(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  Y = AC;
  incrementPC(0x01);
}
// - TYA (Transfer Y to A)
void Cpu::TYA(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  AC = Y;
  incrementPC(0x01);
}
// - DEY (DEcrement Y)
void Cpu::DEY(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  Y -= 0x01;
  incrementPC(0x01);
}
// - INY (INcrement Y)
void Cpu::INY(uint16_t (Cpu::*AddressingMode)()) {
  static_cast<void>(AddressingMode);
  Y += 0x01;
  incrementPC(0x01);
}
// ROL (ROtate Left)
// ROR (ROtate Right)
// RTI (ReTurn from Interrupt)
// RTS (ReTurn from Subroutine)
// SBC (SuBtract with Carry)
// STA (STore Accumulator)
// Stack Instructions
// STX (STore X register)
void Cpu::STX(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  memory.write(address, X);
  incrementPC(0x01);
}
// STY (STore Y register)
void Cpu::STY(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  memory.write(address, Y);
  incrementPC(0x01);
}
