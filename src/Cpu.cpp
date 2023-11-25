#include "Cpu.hpp"
#include <cstdint>
#include <ios>

Cpu::Cpu(Memory &memory, uint16_t PC, uint8_t SP, uint8_t AC, uint8_t X,
         uint8_t Y, uint8_t SR)
    : memory(memory), PC(PC), SP(SP), AC(AC), X(X), Y(Y), SR(SR) {}

Cpu::~Cpu() {}

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

void Cpu::next() {

  // adc_im(0x07, 2);
  // adc_zp(0x01);
  // adc_zpx(0x02);
  // adc_abs(0x03);
  // adc_absx(0x04);
  // adc_absy(0x05);
  // adc_indx(0x06);
  // adc_indy(0x07);
  //
  stx_zp(0x00);
}

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
void Cpu::adc_im(uint8_t value, uint8_t instructionSize = 0x02) {
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

  incrementPC(instructionSize);
}

void Cpu::adc_zp(uint8_t address) {
  uint8_t value = zeropage(address);
  adc_im(value, 0x02);
}

void Cpu::adc_zpx(uint8_t address) {
  uint8_t value = zeropage(address + X);
  adc_im(value, 0x02);
}

void Cpu::adc_abs(uint16_t address) {
  uint8_t value = absolute(address);
  adc_im(value, 0x03);
}

void Cpu::adc_absx(uint16_t address) {
  uint16_t value = absolute(address + X);
  adc_im(value, 0x03);
}

void Cpu::adc_absy(uint16_t address) {
  uint16_t value = absolute(address + Y);
  adc_im(value, 0x03);
}

void Cpu::adc_indx(uint8_t address) {
  uint16_t value = indirectX(address);
  adc_im(value, 0x02);
}

void Cpu::adc_indy(uint8_t address) {
  uint16_t value = indirectY(address);
  adc_im(value, 0x02);
}

// -- STX (STore X register) ------------------------------------ //

// Armazena o valor contido no registrador X no endereço do operando (zeropage).
void Cpu::stx_zp(uint8_t address) {
  memory.write(address, X);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador X no endereço do operando + Y.
void Cpu::stx_zpy(uint8_t address) {
  memory.write(address + Y, X);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador X no endereço absoluto do operando.
void Cpu::stx_abs(uint8_t address) {
  memory.write(address, X);
  incrementPC(0x03);
}

// -- STY (STore X register) ------------------------------------ //

// Armazena o valor contido no registrador Y no endereço do operando (zeropage).
void Cpu::sty_zp(uint8_t address) {
  memory.write(address, Y);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador Y no endereço do operando + X.
void Cpu::sty_zpx(uint8_t address) {
  memory.write(address + X, Y);
  incrementPC(0x02);
}

// Armazena o valor contido no registrador Y no endereço absoluto do operando.
void Cpu::sty_abs(uint8_t address) {
  memory.write(address, Y);
  incrementPC(0x03);
}
