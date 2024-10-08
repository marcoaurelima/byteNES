#include "Cpu.hpp"
#include <bitset>
#include <cmath>
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

  /*// ADC (ADd with Carry)*/
  opcodeMapping[0x69] = &Cpu::ADC;
  opcodeMapping[0x65] = &Cpu::ADC;
  opcodeMapping[0x75] = &Cpu::ADC;
  opcodeMapping[0x6D] = &Cpu::ADC;
  opcodeMapping[0x7D] = &Cpu::ADC;
  opcodeMapping[0x79] = &Cpu::ADC;
  opcodeMapping[0x61] = &Cpu::ADC;
  opcodeMapping[0x71] = &Cpu::ADC;
  opcodeInfo[0x69] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0x65] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x75] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0x6D] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0x7D] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0x79] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0x61] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0x71] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// AND (bitwise AND with accumulator)*/
  opcodeMapping[0x29] = &Cpu::AND;
  opcodeMapping[0x25] = &Cpu::AND;
  opcodeMapping[0x35] = &Cpu::AND;
  opcodeMapping[0x2D] = &Cpu::AND;
  opcodeMapping[0x3D] = &Cpu::AND;
  opcodeMapping[0x39] = &Cpu::AND;
  opcodeMapping[0x21] = &Cpu::AND;
  opcodeMapping[0x31] = &Cpu::AND;
  opcodeInfo[0x29] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0x25] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x35] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0x2D] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0x3D] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0x39] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0x21] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0x31] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// ASL (Arithmetic Shift Left)*/
  opcodeMapping[0x0A] = &Cpu::ASL_AC;
  opcodeMapping[0x06] = &Cpu::ASL;
  opcodeMapping[0x16] = &Cpu::ASL;
  opcodeMapping[0x0E] = &Cpu::ASL;
  opcodeMapping[0x1E] = &Cpu::ASL;
  opcodeInfo[0x0A] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x06] = {ADDR_MODE::ZEROPAGE, 5, 0};
  opcodeInfo[0x16] = {ADDR_MODE::ZEROPAGE_X, 6, 0};
  opcodeInfo[0x0E] = {ADDR_MODE::ABSOLUTE, 6, 0};
  opcodeInfo[0x1E] = {ADDR_MODE::ABSOLUTE_X, 7, 0};
  /*// BIT (test BITs)*/
  opcodeMapping[0x24] = &Cpu::BIT;
  opcodeMapping[0x2C] = &Cpu::BIT;
  opcodeInfo[0x24] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x2C] = {ADDR_MODE::ABSOLUTE, 4, 0};
  /*// Branch Instructions*/
  opcodeMapping[0x10] = &Cpu::BPL;
  opcodeMapping[0x30] = &Cpu::BMI;
  opcodeMapping[0x50] = &Cpu::BVC;
  opcodeMapping[0x70] = &Cpu::BVS;
  opcodeMapping[0x90] = &Cpu::BCC;
  opcodeMapping[0xB0] = &Cpu::BCS;
  opcodeMapping[0xD0] = &Cpu::BNE;
  opcodeMapping[0xF0] = &Cpu::BEQ;
  opcodeInfo[0x10] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0x30] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0x50] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0x70] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0x90] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0xB0] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0xD0] = {ADDR_MODE::RELATIVE, 2, 1};
  opcodeInfo[0xF0] = {ADDR_MODE::RELATIVE, 2, 1};
  /*// BRK (BReaK)*/
  opcodeMapping[0x00] = &Cpu::BRK;
  opcodeInfo[0x00] = {ADDR_MODE::NONE, 7, 0};
  /*// CMP (CoMPare accumulator)*/
  opcodeMapping[0xC9] = &Cpu::CMP;
  opcodeMapping[0xC5] = &Cpu::CMP;
  opcodeMapping[0xD5] = &Cpu::CMP;
  opcodeMapping[0xCD] = &Cpu::CMP;
  opcodeMapping[0xDD] = &Cpu::CMP;
  opcodeMapping[0xD9] = &Cpu::CMP;
  opcodeMapping[0xC1] = &Cpu::CMP;
  opcodeMapping[0xD1] = &Cpu::CMP;
  opcodeInfo[0xC9] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xC5] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xD5] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0xCD] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0xDD] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0xD9] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0xC1] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0xD1] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// CPX (ComPare X register)*/
  opcodeMapping[0xE0] = &Cpu::CPX;
  opcodeMapping[0xE4] = &Cpu::CPX;
  opcodeMapping[0xEC] = &Cpu::CPX;
  opcodeInfo[0xE0] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xE4] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xEC] = {ADDR_MODE::ABSOLUTE, 4, 0};
  /*// CPY (ComPare Y register)*/
  opcodeMapping[0xC0] = &Cpu::CPY;
  opcodeMapping[0xC4] = &Cpu::CPY;
  opcodeMapping[0xCC] = &Cpu::CPY;
  opcodeInfo[0xC0] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xC4] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xCC] = {ADDR_MODE::ABSOLUTE, 4, 0};
  /*// DEC (DECrement memory)*/
  opcodeMapping[0xC6] = &Cpu::DEC;
  opcodeMapping[0xD6] = &Cpu::DEC;
  opcodeMapping[0xCE] = &Cpu::DEC;
  opcodeMapping[0xDE] = &Cpu::DEC;
  opcodeInfo[0xC6] = {ADDR_MODE::ZEROPAGE, 5, 0};
  opcodeInfo[0xD6] = {ADDR_MODE::ZEROPAGE_X, 6, 0};
  opcodeInfo[0xCE] = {ADDR_MODE::ABSOLUTE, 6, 0};
  opcodeInfo[0xDE] = {ADDR_MODE::ABSOLUTE_X, 7, 0};
  /*// EOR (bitwise Exclusive OR)*/
  opcodeMapping[0x49] = &Cpu::EOR;
  opcodeMapping[0x45] = &Cpu::EOR;
  opcodeMapping[0x55] = &Cpu::EOR;
  opcodeMapping[0x4D] = &Cpu::EOR;
  opcodeMapping[0x5D] = &Cpu::EOR;
  opcodeMapping[0x59] = &Cpu::EOR;
  opcodeMapping[0x41] = &Cpu::EOR;
  opcodeMapping[0x51] = &Cpu::EOR;
  opcodeInfo[0x49] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0x45] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x55] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0x4D] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0x5D] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0x59] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0x41] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0x51] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// Flag (Processor Status) Instructions*/
  opcodeMapping[0x18] = &Cpu::CLC;
  opcodeMapping[0x38] = &Cpu::SEC;
  opcodeMapping[0x58] = &Cpu::CLI;
  opcodeMapping[0x78] = &Cpu::SEI;
  opcodeMapping[0xB8] = &Cpu::CLV;
  opcodeMapping[0xD8] = &Cpu::CLD;
  opcodeMapping[0xF8] = &Cpu::SED;
  opcodeInfo[0x18] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x38] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x58] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x78] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xB8] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xD8] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xF8] = {ADDR_MODE::NONE, 2, 0};
  /*// INC (INCrement memory)*/
  opcodeMapping[0xE6] = &Cpu::INC;
  opcodeMapping[0xF6] = &Cpu::INC;
  opcodeMapping[0xEE] = &Cpu::INC;
  opcodeMapping[0xFE] = &Cpu::INC;
  opcodeInfo[0xE6] = {ADDR_MODE::ZEROPAGE, 5, 0};
  opcodeInfo[0xF6] = {ADDR_MODE::ZEROPAGE_X, 6, 0};
  opcodeInfo[0xEE] = {ADDR_MODE::ABSOLUTE, 6, 0};
  opcodeInfo[0xFE] = {ADDR_MODE::ABSOLUTE_X, 7, 0};
  /*// JMP (JuMP)*/
  opcodeMapping[0x4C] = &Cpu::JMP;
  opcodeMapping[0x6C] = &Cpu::JMP;
  opcodeInfo[0x4C] = {ADDR_MODE::ABSOLUTE, 3, 0};
  opcodeInfo[0x6C] = {ADDR_MODE::INDIRECT, 5, 0};
  /*// JSR (Jump to SubRoutine)*/
  opcodeMapping[0x20] = &Cpu::JSR;
  opcodeInfo[0x20] = {ADDR_MODE::ABSOLUTE, 6, 0};
  /**/
  /*// LDA (LoaD Accumulator)*/
  opcodeMapping[0xA9] = &Cpu::LDA;
  opcodeMapping[0xA5] = &Cpu::LDA;
  opcodeMapping[0xB5] = &Cpu::LDA;
  opcodeMapping[0xAD] = &Cpu::LDA;
  opcodeMapping[0xBD] = &Cpu::LDA;
  opcodeMapping[0xB9] = &Cpu::LDA;
  opcodeMapping[0xA1] = &Cpu::LDA;
  opcodeMapping[0xB1] = &Cpu::LDA;
  opcodeInfo[0xA9] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xA5] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xB5] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0xAD] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0xBD] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0xB9] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0xA1] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0xB1] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// LDX (LoaD X register)*/
  opcodeMapping[0xA2] = &Cpu::LDX;
  opcodeMapping[0xA6] = &Cpu::LDX;
  opcodeMapping[0xB6] = &Cpu::LDX;
  opcodeMapping[0xAE] = &Cpu::LDX;
  opcodeMapping[0xBE] = &Cpu::LDX;
  opcodeInfo[0xA2] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xA6] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xB6] = {ADDR_MODE::ZEROPAGE_Y, 4, 0};
  opcodeInfo[0xAE] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0xBE] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  /*// LDY (LoaD Y register)*/
  opcodeMapping[0xA0] = &Cpu::LDY;
  opcodeMapping[0xA4] = &Cpu::LDY;
  opcodeMapping[0xB4] = &Cpu::LDY;
  opcodeMapping[0xAC] = &Cpu::LDY;
  opcodeMapping[0xBC] = &Cpu::LDY;
  opcodeInfo[0xA0] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xA4] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xB4] = {ADDR_MODE::ZEROPAGE_Y, 4, 0};
  opcodeInfo[0xAC] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0xBC] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  /*// LSR (Logical Shift Right)*/
  opcodeMapping[0x4A] = &Cpu::LSR_AC;
  opcodeMapping[0x46] = &Cpu::LSR;
  opcodeMapping[0x56] = &Cpu::LSR;
  opcodeMapping[0x4E] = &Cpu::LSR;
  opcodeMapping[0x5E] = &Cpu::LSR;
  opcodeInfo[0x4A] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x46] = {ADDR_MODE::ZEROPAGE, 5, 0};
  opcodeInfo[0x56] = {ADDR_MODE::ZEROPAGE_X, 6, 0};
  opcodeInfo[0x4E] = {ADDR_MODE::ABSOLUTE, 6, 0};
  opcodeInfo[0x5E] = {ADDR_MODE::ABSOLUTE_X, 7, 0};
  /*// NOP (No OPeration)*/
  opcodeMapping[0xEA] = &Cpu::NOP;
  opcodeInfo[0xEA] = {ADDR_MODE::NONE, 2, 0};
  /*// ORA (bitwise OR with Accumulator)*/
  opcodeMapping[0x09] = &Cpu::ORA;
  opcodeMapping[0x05] = &Cpu::ORA;
  opcodeMapping[0x15] = &Cpu::ORA;
  opcodeMapping[0x0D] = &Cpu::ORA;
  opcodeMapping[0x1D] = &Cpu::ORA;
  opcodeMapping[0x19] = &Cpu::ORA;
  opcodeMapping[0x01] = &Cpu::ORA;
  opcodeMapping[0x11] = &Cpu::ORA;
  opcodeInfo[0x09] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0x05] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x15] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0x0D] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0x1D] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0x19] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0x01] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0x11] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// Register Instructions*/
  opcodeMapping[0xAA] = &Cpu::TAX;
  opcodeMapping[0x8A] = &Cpu::TXA;
  opcodeMapping[0xCA] = &Cpu::DEX;
  opcodeMapping[0xE8] = &Cpu::INX;
  opcodeMapping[0xA8] = &Cpu::TAY;
  opcodeMapping[0x98] = &Cpu::TYA;
  opcodeMapping[0x88] = &Cpu::DEY;
  opcodeMapping[0xC8] = &Cpu::INY;
  opcodeInfo[0xAA] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x8A] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xCA] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xE8] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xA8] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x98] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x88] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xC8] = {ADDR_MODE::NONE, 2, 0};
  /*// ROL (ROtate Left)*/
  opcodeMapping[0x2A] = &Cpu::ROL_AC;
  opcodeMapping[0x26] = &Cpu::ROL;
  opcodeMapping[0x36] = &Cpu::ROL;
  opcodeMapping[0x2E] = &Cpu::ROL;
  opcodeMapping[0x3E] = &Cpu::ROL;
  opcodeInfo[0x2A] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x26] = {ADDR_MODE::ZEROPAGE, 5, 0};
  opcodeInfo[0x36] = {ADDR_MODE::ZEROPAGE_X, 6, 0};
  opcodeInfo[0x2E] = {ADDR_MODE::ABSOLUTE, 6, 0};
  opcodeInfo[0x3E] = {ADDR_MODE::ABSOLUTE_X, 7, 0};
  /*// ROR (ROtate Right)*/
  opcodeMapping[0x6A] = &Cpu::ROR_AC;
  opcodeMapping[0x66] = &Cpu::ROR;
  opcodeMapping[0x76] = &Cpu::ROR;
  opcodeMapping[0x6E] = &Cpu::ROR;
  opcodeMapping[0x7E] = &Cpu::ROR;
  opcodeInfo[0x6A] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x66] = {ADDR_MODE::ZEROPAGE, 5, 0};
  opcodeInfo[0x76] = {ADDR_MODE::ZEROPAGE_X, 6, 0};
  opcodeInfo[0x6E] = {ADDR_MODE::ABSOLUTE, 6, 0};
  opcodeInfo[0x7E] = {ADDR_MODE::ABSOLUTE_X, 7, 0};
  /*// RTI (ReTurn from Interrupt)*/
  opcodeMapping[0x40] = &Cpu::RTI;
  opcodeInfo[0x40] = {ADDR_MODE::NONE, 6, 0};
  // RTS (ReTurn from Subroutine)
  opcodeMapping[0x60] = &Cpu::RTS;
  opcodeInfo[0x60] = {ADDR_MODE::NONE, 6, 0};
  // SBC (SuBtract with Carry)
  opcodeMapping[0xE9] = &Cpu::SBC;
  opcodeMapping[0xE5] = &Cpu::SBC;
  opcodeMapping[0xF5] = &Cpu::SBC;
  opcodeMapping[0xED] = &Cpu::SBC;
  opcodeMapping[0xFD] = &Cpu::SBC;
  opcodeMapping[0xF9] = &Cpu::SBC;
  opcodeMapping[0xE1] = &Cpu::SBC;
  opcodeMapping[0xF1] = &Cpu::SBC;
  opcodeInfo[0xE9] = {ADDR_MODE::IMMEDIATE, 2, 0};
  opcodeInfo[0xE5] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0xF5] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0xED] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0xFD] = {ADDR_MODE::ABSOLUTE_X, 4, 1};
  opcodeInfo[0xF9] = {ADDR_MODE::ABSOLUTE_Y, 4, 1};
  opcodeInfo[0xE1] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0xF1] = {ADDR_MODE::INDIRECT_Y, 5, 1};
  /*// STA (STore Accumulator)*/
  opcodeMapping[0x85] = &Cpu::STA;
  opcodeMapping[0x95] = &Cpu::STA;
  opcodeMapping[0x8D] = &Cpu::STA;
  opcodeMapping[0x9D] = &Cpu::STA;
  opcodeMapping[0x99] = &Cpu::STA;
  opcodeMapping[0x81] = &Cpu::STA;
  opcodeMapping[0x91] = &Cpu::STA;
  opcodeInfo[0x85] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x95] = {ADDR_MODE::ZEROPAGE_X, 4, 0};
  opcodeInfo[0x8D] = {ADDR_MODE::ABSOLUTE, 4, 0};
  opcodeInfo[0x9D] = {ADDR_MODE::ABSOLUTE_X, 5, 0};
  opcodeInfo[0x99] = {ADDR_MODE::ABSOLUTE_Y, 5, 0};
  opcodeInfo[0x81] = {ADDR_MODE::INDIRECT_X, 6, 0};
  opcodeInfo[0x91] = {ADDR_MODE::INDIRECT_Y, 6, 0};
  /*// Stack Instructions*/
  opcodeMapping[0x9A] = &Cpu::TXS;
  opcodeMapping[0xBA] = &Cpu::TSX;
  /*static_cast<void>(Addressingmode);*/
  /*auto response = getValueAddrMode(params.addrMode);*/
  opcodeMapping[0x48] = &Cpu::PHA;
  opcodeMapping[0x68] = &Cpu::PLA;
  opcodeMapping[0x08] = &Cpu::PHP;
  opcodeMapping[0x28] = &Cpu::PLP;
  opcodeInfo[0x9A] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0xBA] = {ADDR_MODE::NONE, 2, 0};
  opcodeInfo[0x48] = {ADDR_MODE::NONE, 3, 0};
  opcodeInfo[0x68] = {ADDR_MODE::NONE, 4, 0};
  opcodeInfo[0x08] = {ADDR_MODE::NONE, 3, 0};
  opcodeInfo[0x28] = {ADDR_MODE::NONE, 4, 0};
  /*// STX (STore X register)*/
  opcodeMapping[0x86] = &Cpu::STX;
  opcodeMapping[0x96] = &Cpu::STX;
  opcodeMapping[0x8E] = &Cpu::STX;
  opcodeInfo[0x86] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x96] = {ADDR_MODE::ZEROPAGE_Y, 4, 0};
  opcodeInfo[0x8E] = {ADDR_MODE::ABSOLUTE, 4, 0};
  /*// STY (STore Y register)*/
  opcodeMapping[0x84] = &Cpu::STY;
  opcodeMapping[0x94] = &Cpu::STY;
  opcodeMapping[0x8C] = &Cpu::STY;
  opcodeInfo[0x84] = {ADDR_MODE::ZEROPAGE, 3, 0};
  opcodeInfo[0x94] = {ADDR_MODE::ZEROPAGE_Y, 4, 0};
  opcodeInfo[0x8C] = {ADDR_MODE::ABSOLUTE, 4, 0};

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

void Cpu::showCpuStatus(uint8_t index, bool showOpcodes) {
  if (showOpcodes) {
    std::cout << "| [" << count << "] " << std::dec;
    std::cout << std::hex << "OPCode: " << (int)index << " ("
              << opcodesNames[index] << ")\n";
  }
  std::cout << "| PC: " << std::setfill('0') << std::hex << std::setw(4)
            << (int)PC << " | SP: " << std::setfill('0') << std::hex
            << std::setw(4) << (int)SP << " | AC: " << std::setfill('0')
            << std::hex << std::setw(4) << (int)AC
            << " | X: " << std::setfill('0') << std::hex << std::setw(4)
            << (int)X << " | Y: " << std::setfill('0') << std::hex
            << std::setw(4) << (int)Y << " | SR: " << std::bitset<8>(SR)
            << "]\n";
  std::cout << "                                                           "
               "NV_BDIZC\n";
}

uint8_t Cpu::next() {
  generateRandomIn0xFE();
  uint8_t index = memory.read(PC);

  return (this->*opcodeMapping[index])(opcodeInfo[index]);
}

void Cpu::reset() {
  memory.reset();
  PC = AC = X = Y = 0x00;
  SR = 0x30;
  SP = 0xFF;
}

bool isDifferentPage(uint16_t addr1, uint16_t addr2) {
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
  if (value_2 < value_1) {
    setFlag(Flag::C);
    return;
  }
  remFlag(Flag::C);
}

// Carry (subtraction)
// Este flag é definido se não houver empréstimo durante a subtração.
void Cpu::flagActivationCMP(uint16_t value_1, uint8_t value_2) {
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

MemoryAccessResult Cpu::getValueAddrMode(ADDR_MODE mode) {
  switch (mode) {
  case ADDR_MODE::IMMEDIATE:
    return immediate();
    break;
  case ADDR_MODE::ZEROPAGE:
    return zeropage();
    break;
  case ADDR_MODE::ZEROPAGE_X:
    return zeropageX();
    break;
  case ADDR_MODE::ZEROPAGE_Y:
    return zeropageY();
    break;
  case ADDR_MODE::ABSOLUTE:
    return absolute();
    break;
  case ADDR_MODE::ABSOLUTE_X:
    return absoluteX();
    break;
  case ADDR_MODE::ABSOLUTE_Y:
    return absoluteY();
    break;
  case ADDR_MODE::INDIRECT:
    return indirect();
    break;
  case ADDR_MODE::INDIRECT_X:
    return indirectX();
    break;
  case ADDR_MODE::INDIRECT_Y:
    return indirectY();
    break;
  case ADDR_MODE::RELATIVE:
    return relative();
    break;
  case ADDR_MODE::NONE:
    return MemoryAccessResult{};
  }
  return MemoryAccessResult{};
}

// Implementações das instruções
// ADC (ADd with Carry)
Cpu::CPUCicles Cpu::ADC(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;
  uint8_t result = AC + value + carry;
  flagActivationC_ovflw(AC + value + carry);
  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(AC, result);
  AC = result;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// AND (bitwise AND with accumulator)
Cpu::CPUCicles Cpu::AND(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t result = value & AC;
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// ASL (Arithmetic Shift Left)
Cpu::CPUCicles Cpu::ASL(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t result = value << 0x01;

  (value & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);

  flagActivationN(result);
  flagActivationZ(result);
  memory.write(PC + 1, result);
  incrementPC(response.size);
  return (params.cycles);
}
// ASL (Arithmetic Shift Left) - Operações diretas no acumulador
Cpu::CPUCicles Cpu::ASL_AC(const opcodeParams &params) {
  uint8_t value = AC;
  uint8_t result = (AC << 0x01);

  (value & (0x01 << 7)) ? setFlag(Flag::C) : remFlag(Flag::C);

  flagActivationN(result);
  flagActivationZ(result);
  AC = value;
  incrementPC(0x01);
  return (params.cycles);
}
// BIT (test BITs)
Cpu::CPUCicles Cpu::BIT(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
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
  return (params.cycles);
}
// Branch Instructions
// - BPL (Branch on PLus) - Desvio quando FlagN = 0
Cpu::CPUCicles Cpu::BPL(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (!chkFlag(Flag::N)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BMI (Branch on MInus) - Desvio quando FlagN = 1
Cpu::CPUCicles Cpu::BMI(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (chkFlag(Flag::N)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BVC (Branch on oVerflow Clear) - Desvio quando FlagV = 0
Cpu::CPUCicles Cpu::BVC(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  if (!chkFlag(Flag::V)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BVS (Branch on oVerflow Set) - Desvio quando FlagV = 1
Cpu::CPUCicles Cpu::BVS(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (chkFlag(Flag::V)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BCC (Branch on Carry Clear) - Desvio quando FlagC = 0
Cpu::CPUCicles Cpu::BCC(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (!chkFlag(Flag::C)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BCS (Branch on Carry Set) - Desvio quando FlagC = 1
Cpu::CPUCicles Cpu::BCS(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (chkFlag(Flag::C)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BNE (Branch on Not Equal) - Desvio quando FlagZ = 0
Cpu::CPUCicles Cpu::BNE(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (!chkFlag(Flag::Z)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// - BEQ (Branch on EQual) - Desvio quando FlagZ = 1
Cpu::CPUCicles Cpu::BEQ(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  if (chkFlag(Flag::Z)) {
    PC = response.address + response.size;
    return (params.cycles + 0x01 +
            (response.pageCrossed ? params.cyclesOnPageCross : 0));
  }

  incrementPC(response.size);
  return (params.cycles);
}
// BRK (BReaK)
Cpu::CPUCicles Cpu::BRK(const opcodeParams &params) {
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
  return (params.cycles);
}
// CMP (CoMPare accumulator)
Cpu::CPUCicles Cpu::CMP(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  flagActivationCMP(AC, value);

  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// CPX (ComPare X register)
Cpu::CPUCicles Cpu::CPX(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  flagActivationCMP(X, value);

  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// CPY (ComPare Y register)
Cpu::CPUCicles Cpu::CPY(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  flagActivationCMP(Y, value);

  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// DEC (DECrement memory)
Cpu::CPUCicles Cpu::DEC(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t result = value - 0x01;
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);

  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// EOR (bitwise Exclusive OR)
Cpu::CPUCicles Cpu::EOR(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t result = AC ^ value;
  flagActivationN(result);
  flagActivationZ(result);

  AC = result;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// Flag (Processor Status) Instructions
/// - CLC (CLear Carry)
Cpu::CPUCicles Cpu::CLC(const opcodeParams &params) {
  remFlag(Flag::C);
  incrementPC(0x01);
  return (params.cycles);
}
// - SEC (SEt Carry)
Cpu::CPUCicles Cpu::SEC(const opcodeParams &params) {
  setFlag(Flag::C);
  incrementPC(0x01);
  return (params.cycles);
}
// - CLI (CLear Interrupt)
Cpu::CPUCicles Cpu::CLI(const opcodeParams &params) {
  remFlag(Flag::I);
  incrementPC(0x01);
  return (params.cycles);
}
// - SEI (SEt Interrupt)
Cpu::CPUCicles Cpu::SEI(const opcodeParams &params) {
  setFlag(Flag::I);
  incrementPC(0x01);
  return (params.cycles);
}
// - CLV (CLear oVerflow)
Cpu::CPUCicles Cpu::CLV(const opcodeParams &params) {
  remFlag(Flag::V);
  incrementPC(0x01);
  return (params.cycles);
}
// - CLD (CLear Decimal)
Cpu::CPUCicles Cpu::CLD(const opcodeParams &params) {
  remFlag(Flag::D);
  incrementPC(0x01);
  return (params.cycles);
}
// - SED (SEt Decimal)
Cpu::CPUCicles Cpu::SED(const opcodeParams &params) {
  setFlag(Flag::D);
  incrementPC(0x01);
  return (params.cycles);
}
// INC (INCrement memory)
Cpu::CPUCicles Cpu::INC(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t result = value + 0x01;
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// JMP (JuMP)  [ok]Teste 1
Cpu::CPUCicles Cpu::JMP(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  PC = response.address;
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// JSR (Jump to SubRoutine) - Salva o end. de Retorno na pilha
Cpu::CPUCicles Cpu::JSR(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);

  uint16_t nextOP = PC + response.size;
  uint8_t nextOP_lsb = static_cast<uint8_t>(nextOP & 0x00FF);
  uint8_t nextOP_msb = static_cast<uint8_t>(nextOP >> 0x08);

  stackPUSH(nextOP_lsb);
  stackPUSH(nextOP_msb);

  PC = response.address;
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}

// LDA (LoaD Accumulator)
Cpu::CPUCicles Cpu::LDA(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  AC = value;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// LDX (LoaD X register)ADC #$0F
Cpu::CPUCicles Cpu::LDX(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  X = value;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// LDY (LoaD Y register)
Cpu::CPUCicles Cpu::LDY(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  flagActivationN(value);
  flagActivationZ(value);
  Y = value;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// LSR (Logical Shift Right)
Cpu::CPUCicles Cpu::LSR(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  (value & 0x01) ? setFlag(Flag::C) : remFlag(Flag::C);
  uint8_t result = (value >> 0x01);
  flagActivationN(result);
  flagActivationZ(result);
  memory.write(response.address, result);
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
Cpu::CPUCicles Cpu::LSR_AC(const opcodeParams &params) {
  (AC & 0x01) ? setFlag(Flag::C) : remFlag(Flag::C);
  uint8_t result = (AC >> 0x01);
  flagActivationN(result);
  flagActivationZ(result);
  AC = result;
  incrementPC(0x01);
  return (params.cycles);
}
// NOP (No OPeration)
Cpu::CPUCicles Cpu::NOP(const opcodeParams &params) {
  incrementPC(0x01);
  return (params.cycles);
}
// ORA (bitwise OR with Accumulator)
Cpu::CPUCicles Cpu::ORA(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t result = AC | value;
  flagActivationN(result);
  flagActivationZ(result);

  AC = result;
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// Register Instructions
// - TAX (Transfer A to X)
Cpu::CPUCicles Cpu::TAX(const opcodeParams &params) {
  X = AC;
  flagActivationN(AC);
  flagActivationZ(AC);
  incrementPC(0x01);
  return (params.cycles);
}
// - TXA (Transfer X to A)
Cpu::CPUCicles Cpu::TXA(const opcodeParams &params) {
  AC = X;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
  return (params.cycles);
}
// - DEX (DEcrement X)
Cpu::CPUCicles Cpu::DEX(const opcodeParams &params) {
  X -= 0x01;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
  return (params.cycles);
}
// - INX (INcrement X)
Cpu::CPUCicles Cpu::INX(const opcodeParams &params) {
  X += 0x01;
  flagActivationN(X);
  flagActivationZ(X);
  incrementPC(0x01);
  return (params.cycles);
}
// - TAY (Transfer A to Y)
Cpu::CPUCicles Cpu::TAY(const opcodeParams &params) {
  Y = AC;
  flagActivationN(AC);
  flagActivationZ(AC);
  incrementPC(0x01);
  return (params.cycles);
}
// - TYA (Transfer Y to A)
Cpu::CPUCicles Cpu::TYA(const opcodeParams &params) {
  AC = Y;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
  return (params.cycles);
}
// - DEY (DEcrement Y)
Cpu::CPUCicles Cpu::DEY(const opcodeParams &params) {
  Y -= 0x01;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
  return (params.cycles);
}
// - INY (INcrement Y)
Cpu::CPUCicles Cpu::INY(const opcodeParams &params) {
  Y += 0x01;
  flagActivationN(Y);
  flagActivationZ(Y);
  incrementPC(0x01);
  return (params.cycles);
}
// ROL (ROtate Left)
Cpu::CPUCicles Cpu::ROL(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
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
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
Cpu::CPUCicles Cpu::ROL_AC(const opcodeParams &params) {

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
  return (params.cycles);
}
// ROR (ROtate Right)
Cpu::CPUCicles Cpu::ROR(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
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
  return (params.cycles);
}
Cpu::CPUCicles Cpu::ROR_AC(const opcodeParams &params) {
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
  return (params.cycles);
}
// RTI (ReTurn from Interrupt)
Cpu::CPUCicles Cpu::RTI(const opcodeParams &params) {
  /*static_cast<void>(Addressingmode);*/
  uint8_t PC_msb = stackPOP();
  uint8_t PC_lsb = stackPOP();

  SR = stackPOP();
  PC = (PC_msb << 8) | PC_lsb;
  return (params.cycles);
}
// RTS (ReTurn from Subroutine)
Cpu::CPUCicles Cpu::RTS(const opcodeParams &params) {
  uint8_t address_msb = stackPOP();
  uint8_t address_lsb = stackPOP();
  uint16_t address = (address_msb << 0x08) | address_lsb;
  PC = address;
  return (params.cycles + params.cyclesOnPageCross);
}
// SBC (SuBtract with Carry)
Cpu::CPUCicles Cpu::SBC(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  uint8_t value = memory.read(response.address);
  uint8_t carry = chkFlag(Flag::C) ? 0x01 : 0x00;

  uint16_t result = AC - value - (1 - carry);

  flagActivationC_unflw(AC, AC - value - (1 - carry));

  flagActivationN(result);
  flagActivationZ(result);
  flagActivationV(AC, result);
  AC = result;
  incrementPC(response.size);
  return (params.cycles);
}

// STA (STore Accumulator)
Cpu::CPUCicles Cpu::STA(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  memory.write(response.address, AC);
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// Stack Instructions
// - TXS (Transfer X to Stack ptr)
Cpu::CPUCicles Cpu::TXS(const opcodeParams &params) {
  SP = X;
  incrementPC(0x01);
  return (params.cycles);
}
// - TSX (Transfer Stack ptr to X)
Cpu::CPUCicles Cpu::TSX(const opcodeParams &params) {
  X = SP;
  incrementPC(0x01);
  return (params.cycles);
}
// - PHA (PusH Accumulator)
Cpu::CPUCicles Cpu::PHA(const opcodeParams &params) {
  stackPUSH(AC);
  incrementPC(0x01);
  return (params.cycles);
}
// - PLA (PuLl Accumulator)
Cpu::CPUCicles Cpu::PLA(const opcodeParams &params) {
  AC = stackPOP();
  incrementPC(0x01);
  return (params.cycles);
}
// - PHP (PusH Processor status)
Cpu::CPUCicles Cpu::PHP(const opcodeParams &params) {
  stackPUSH(SR);
  incrementPC(0x01);
  return (params.cycles);
}
// - PLP (PuLl Processor status)
Cpu::CPUCicles Cpu::PLP(const opcodeParams &params) {
  SR = stackPOP();
  incrementPC(0x01);
  return (params.cycles);
}
// STX (STore X register)
Cpu::CPUCicles Cpu::STX(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  memory.write(response.address, X);
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
// STY (STore Y register)
Cpu::CPUCicles Cpu::STY(const opcodeParams &params) {
  auto response = getValueAddrMode(params.addrMode);
  memory.write(response.address, Y);
  incrementPC(response.size);
  return (params.cycles +
          (response.pageCrossed ? params.cyclesOnPageCross : 0));
}
