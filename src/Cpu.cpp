#include "Cpu.hpp"
#include <cstdint>
#include <ios>

Cpu::Cpu(Memory &memory, uint16_t PC, uint8_t SP, uint8_t AC, uint8_t X,
         uint8_t Y, uint8_t SR)
    : memory(memory), PC(PC), SP(SP), AC(AC), X(X), Y(Y), SR(SR) {
  fillOpcodeMapping();
}

Cpu::~Cpu() {}

void Cpu::fillOpcodeMapping() {
  // - Usando funções lambda para evitar complexidade relativo a
  // endereços de memória de funções membro.
  // - As funções lambda tem a mesma assinatura por causa do array de
  // funções, mesmo que as funçoes internas não usem o parametro 'v'

  // ADC (ADd with Carry)
  opcodeMapping[0x69] = [this](uint16_t v) { this->adc_im(v); };
  opcodeMapping[0x65] = [this](uint16_t v) { this->adc_zp(); };
  opcodeMapping[0x75] = [this](uint16_t v) { this->adc_zpx(); };
  opcodeMapping[0x6D] = [this](uint16_t v) { this->adc_abs(); };
  opcodeMapping[0x7D] = [this](uint16_t v) { this->adc_absx(); };
  opcodeMapping[0x79] = [this](uint16_t v) { this->adc_absy(); };
  opcodeMapping[0x61] = [this](uint16_t v) { this->adc_indx(); };
  opcodeMapping[0x71] = [this](uint16_t v) { this->adc_indy(); };

  // AND (bitwise AND with accumulator)
  // ASL (Arithmetic Shift Left)
  // BIT (test BITs)
  // Branch Instructions
  // BRK (BReaK)
  // CMP (CoMPare accumulator)
  // CPX (ComPare X register)
  // CPY (ComPare Y regiarch?cache=wsyo1o9wyksi3aq9b7bnz8x5ster)
  // DEC (DECrement memory)
  // EOR (bitwise Exclusive OR)
  // Flag (Processor Status) Instructions
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
  opcodeMapping[0x86] = [this](uint16_t v) { this->stx_zp(); };
  opcodeMapping[0x96] = [this](uint16_t v) { this->stx_zpy(); };
  opcodeMapping[0x8E] = [this](uint16_t v) { this->stx_abs(); };
  // STY (STore Y register)
  opcodeMapping[0x84] = [this](uint16_t v) { this->sty_zp(); };
  opcodeMapping[0x94] = [this](uint16_t v) { this->sty_zpx(); };
  opcodeMapping[0x8C] = [this](uint16_t v) { this->sty_abs(); };
}

uint16_t Cpu::getPC() { return PC; }
uint8_t Cpu::getSP() { return SP; }
uint8_t Cpu::getAC() { return AC; }
uint8_t Cpu::getX() { return X; }
uint8_t Cpu::getY() { return Y; }
uint8_t Cpu::getSR() { return SR; }
Memory &Cpu::getMemory() { return memory; }

void Cpu::setFlag(Flag flag) {
  switch (flag) {
  case Flag::N:
    SR = SR | 0b10000000;
    break;
  case Flag::V:
    SR = SR | 0b01000000;
    break;
  case Flag::B:
    SR = SR | 0b00010000;
    break;
  case Flag::D:
    SR = SR | 0b00001000;
    break;
  case Flag::I:
    SR = SR | 0b00000100;
    break;
  case Flag::Z:
    SR = SR | 0b00000010;
    break;
  case Flag::C:
    SR = SR | 0b00000001;
    break;
  default:
    break;
  }
}

void Cpu::incrementPC(uint8_t value) { PC += value; }

void Cpu::next() {}

void Cpu::reset() {
  memory.reset();
  PC = SP = AC = X = Y = SR = 0X00;
}

// Concatena 2 variaveis de 8 bits em uma única de 16 bits.
uint16_t concat2Bytes(uint8_t msb, uint8_t lsb) { return (msb << 8) | lsb; }

// Modos de endereçamento
uint8_t Cpu::immediate(uint8_t &value) { return value; }

uint8_t Cpu::zeropage(uint8_t address) { return memory.read(address); }

uint8_t Cpu::absolute(uint16_t address) { return memory.read(address); }

uint8_t Cpu::indirectX(uint8_t address) {
  uint8_t byte0 = memory.read(address + X);
  uint8_t byte1 = memory.read(address + X + 0x01);
  uint16_t addr = concat2Bytes(byte1, byte0);

  return memory.read(addr);
}

uint8_t Cpu::indirectY(uint8_t address) {
  uint8_t byte0 = memory.read(address);
  uint8_t byte1 = memory.read(address + 0x01);
  uint16_t addr = concat2Bytes(byte1, byte0);

  return memory.read(addr + Y);
}

// --ADC (ADd with Carry) ------------------------------------ //

// Adiciona o valor imediato diretamente ao registrador acumulador
void Cpu::adc_im(uint8_t value) {
  uint8_t result = AC + immediate(value);

  if (result & 0b10000000) {
    setFlag(Flag::N);
  }

  if (result == 0) {
    setFlag(Flag::Z);
  }

  if (result <= AC) {
    setFlag(Flag::C);
  }

  if (((AC ^ value) & 0b10000000) && ((AC ^ result) & 0b10000000)) {
    setFlag(Flag::V);
  }

  AC = result;

  incrementPC(0x02);
}

void Cpu::adc_zp() {
  uint8_t address = memory.read(PC + 1);
  uint8_t value = zeropage(address);
  adc_im(value);
}

void Cpu::adc_zpx() {
  uint8_t address = memory.read(PC + 1);
  uint8_t value = zeropage(address + X);
  adc_im(value);
}

void Cpu::adc_abs() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);
  uint8_t value = absolute(address);
  adc_im(value);
  incrementPC(0x01);
}

void Cpu::adc_absx() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);
  uint8_t value = memory.read(address + X);
  adc_im(value);
  incrementPC(0x01);
}

void Cpu::adc_absy() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);
  uint8_t value = memory.read(address + Y);
  adc_im(value);
  incrementPC(0x01);
}

void Cpu::adc_indx() {
  uint8_t address = memory.read(PC + 1);
  uint16_t value = indirectX(address);
  adc_im(value);
}

void Cpu::adc_indy() {
  uint8_t address = memory.read(PC + 1);
  uint16_t value = indirectY(address);
  adc_im(value);
}

// -- STX (STore X register) ------------------------------------ //

// Armazena o valor contido no registrador X no endereço do operando (zeropage).
void Cpu::stx_zp() {
  uint8_t address = memory.read(PC + 1);
  memory.write(address, X);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador X no endereço do operando + Y.
void Cpu::stx_zpy() {
  uint8_t address = memory.read(PC + 1);
  memory.write(address + Y, X);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador X no endereço absoluto do operando.
void Cpu::stx_abs() {
  uint8_t address = memory.read(PC + 1);
  memory.write(address, X);
  incrementPC(0x03);
}

// -- STY (STore X register) ------------------------------------ //

// Armazena o valor contido no registrador Y no endereço do operando (zeropage).
void Cpu::sty_zp() {
  uint8_t address = memory.read(PC + 1);
  memory.write(address, Y);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador Y no endereço do operando + X.
void Cpu::sty_zpx() {
  uint8_t address = memory.read(PC + 1);
  memory.write(address + X, Y);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador Y no endereço absoluto do operando.
void Cpu::sty_abs() {
  uint8_t address = memory.read(PC + 1);
  memory.write(address, Y);
  incrementPC(0x03);
}
