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

  // Flag (Processor Status) Instructions
  opcodeMapping[0x18] = [this]() { this->CLC(nullptr); };
  opcodeMapping[0x38] = [this]() { this->SEC(nullptr); };
  opcodeMapping[0x58] = [this]() { this->CLI(nullptr); };
  opcodeMapping[0x78] = [this]() { this->SEI(nullptr); };
  opcodeMapping[0xB8] = [this]() { this->CLV(nullptr); };
  opcodeMapping[0xD8] = [this]() { this->CLD(nullptr); };
  opcodeMapping[0xF8] = [this]() { this->SED(nullptr); };

  /*

  // ADC (ADd with Carry)
  opcodeMapping[0x69] = [this]() { this->adc_im(); };
  opcodeMapping[0x65] = [this]() { this->adc_zp(); };
  opcodeMapping[0x75] = [this]() { this->adc_zpx(); };
  opcodeMapping[0x6D] = [this]() { this->adc_abs(); };
  opcodeMapping[0x7D] = [this]() { this->adc_absx(); };
  opcodeMapping[0x79] = [this]() { this->adc_absy(); };
  opcodeMapping[0x61] = [this]() { this->adc_indx(); };
  opcodeMapping[0x71] = [this]() { this->adc_indy(); };

  // AND (bitwise AND with accumulator)
  opcodeMapping[0x29] = [this]() { this->and_im(); };
  opcodeMapping[0x25] = [this]() { this->and_zp(); };
  opcodeMapping[0x35] = [this]() { this->and_zpx(); };
  opcodeMapping[0x2D] = [this]() { this->and_abs(); };
  opcodeMapping[0x3D] = [this]() { this->and_absx(); };
  opcodeMapping[0x39] = [this]() { this->and_absy(); };
  opcodeMapping[0x21] = [this]() { this->and_indx(); };
  opcodeMapping[0x31] = [this]() { this->and_indy(); };

  // ASL (Arithmetic Shift Left)
  opcodeMapping[0x0A] = [this]() { this->asl_acc(); };
  opcodeMapping[0x06] = [this]() { this->asl_zp(); };
  opcodeMapping[0x16] = [this]() { this->asl_zpx(); };
  opcodeMapping[0x0E] = [this]() { this->asl_abs(); };
  opcodeMapping[0x1E] = [this]() { this->asl_absx(); };

  // BIT (test BITs)
  // Branch Instructions
  // BRK (BReaK)
  // CMP (CoMPare accumulator)
  // CPX (ComPare X register)
  // CPY (ComPare Y regiarch?cache=wsyo1o9wyksi3aq9b7bnz8x5ster)
  // DEC (DECrement memory)
  // EOR (bitwise Exclusive OR)
  // Flag (Processor Status) Instructions
  opcodeMapping[0x18] = [this]() { this->clc(); };
  opcodeMapping[0x38] = [this]() { this->sec(); };
  opcodeMapping[0x58] = [this]() { this->cli(); };
  opcodeMapping[0x78] = [this]() { this->sei(); };
  opcodeMapping[0xB8] = [this]() { this->clv(); };
  opcodeMapping[0xD8] = [this]() { this->cld(); };
  opcodeMapping[0xF8] = [this]() { this->sed(); };

  // INC (INCrement memory)
  // JMP (JuMP)
  // JSR (Jump to SubRoutine)
  // LDA (LoaD Accumulator)
  // LDX (LoaD X register)
  // LDY (LoaD Y register)
  // LSR (Logical Shift Right)
  // NOP (No OPeration)
  // ORA (bitwise OR with Accumulator)
  // Register Instructions
  // ROL (ROtate Left)
  // ROR (ROtate Right)
  // RTI (ReTurn from Interrupt)
  // RTS (ReTurn from Subroutine)
  // SBC (SuBtract with Carry)
  // STA (STore Accumulator)
  // Stack Instructions
  // STX (STore X register)
  opcodeMapping[0x86] = [this]() { this->stx_zp(); };
  opcodeMapping[0x96] = [this]() { this->stx_zpy(); };
  opcodeMapping[0x8E] = [this]() { this->stx_abs(); };
  // STY (STore Y register)
  opcodeMapping[0x84] = [this]() { this->sty_zp(); };
  opcodeMapping[0x94] = [this]() { this->sty_zpx(); };
  opcodeMapping[0x8C] = [this]() { this->sty_abs(); };
  */
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

// Carry
void Cpu::flagActivationC(uint16_t value) {
  if (value > 0xFF) {
    setFlag(Flag::C);
  }
}

// ADC (ADd with Carry)
void Cpu::ADC(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t result = AC + value;
  flagActivationC(AC + value);
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
  flagActivationC(value & AC);
  flagActivationZ(result);
  flagActivationV(value, result);
  AC = result;
  incrementPC(0x01);
}

// ASL (Arithmetic Shift Left)
void Cpu::ASL(uint16_t (Cpu::*Addressingmode)()) {
  uint16_t address = (this->*Addressingmode)();
  uint8_t value = memory.read(address);
  uint8_t result = value << 0x01;
  flagActivationN(result);
  flagActivationC(value << 0x01);
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
  flagActivationC(AC << 0x01);
  flagActivationZ(result);
  AC = value;
  incrementPC(0x01);
}

// BIT (test BITs)
// Branch Instructions
// BRK (BReaK)
// CMP (CoMPare accumulator)
// CPX (ComPare X register)
// CPY (ComPare Y regiarch?cache=wsyo1o9wyksi3aq9b7bnz8x5ster)
// DEC (DECrement memory)
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
// JMP (JuMP)
// JSR (Jump to SubRoutine)
// LDA (LoaD Accumulator)
// LDX (LoaD X register)
// LDY (LoaD Y register)
// LSR (Logical Shift Right)
// NOP (No OPeration)
// ORA (bitwise OR with Accumulator)
// Register Instructions
// ROL (ROtate Left)
// ROR (ROtate Right)
// RTI (ReTurn from Interrupt)
// RTS (ReTurn from Subroutine)
// SBC (SuBtract with Carry)
// STA (STore Accumulator)
// Stack Instructions
// STX (STore X register)
// STY (STore Y register)
