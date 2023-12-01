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
  // - Usando funções lambda para evitar complexidade relativo a
  // endereços de memória de funções membro.

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

void Cpu::incrementPC(uint16_t value) { PC += value; }

void Cpu::next() {
  uint8_t index = memory.read(PC);
  opcodeMapping[index]();
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

void Cpu::adc_flags_handler(uint8_t value_orig, uint8_t value_new) {
  if (value_new & 0b10000000) {
    setFlag(Flag::N);
  }

  if (value_new == 0) {
    setFlag(Flag::Z);
  }

  if (value_new <= AC) {
    setFlag(Flag::C);
  }

  if (((AC ^ value_orig) & 0b10000000) && ((AC ^ value_new) & 0b10000000)) {
    setFlag(Flag::V);
  }
}

// Soma o valor imediato diretamente no valor contido no registrador acumulador
void Cpu::adc_im() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = AC + immediate(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

// Soma o valor contido em endereço de zero page diretamente no valor contido no
// registrador acumulador
void Cpu::adc_zp() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = AC + zeropage(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

// Soma o valor contido em endereço de zero page + X, diretamente no valor
// contido no registrador acumulador
void Cpu::adc_zpx() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = AC + zeropage(value + X);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

// Soma o valor contido em endereço de 16 bits, diretamente no valor contido no
// registrador acumulador
void Cpu::adc_abs() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address);
  uint8_t result = AC + absolute(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

// Soma o valor contido em endereço de 16 bits + X, diretamente no valor contido
// no registrador acumulador
void Cpu::adc_absx() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + X);
  uint8_t result = AC + absolute(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

// Soma o valor contido em endereço de 16 bits + Y, diretamente no valor contido
// no registrador acumulador
void Cpu::adc_absy() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + Y);
  uint8_t result = AC + absolute(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

void Cpu::adc_indx() {
  uint8_t msb = memory.read(PC + X + 2);
  uint8_t lsb = memory.read(PC + X + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address);
  uint8_t result = AC + absolute(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

void Cpu::adc_indy() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + Y);
  uint8_t result = AC + absolute(value);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
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
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);
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
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);
  memory.write(address, Y);
  incrementPC(0x03);
}

// ++++++++++++++++++++++++ AND Memory with Accumulator
// +++++++++++++++++++++++++++++++ //

void Cpu::and_flags_handler(uint8_t value_orig, uint8_t value_new) {
  if (value_new & 0b10000000) {
    setFlag(Flag::N);
  }

  if (value_new == 0) {
    setFlag(Flag::Z);
  }
}

void Cpu::and_im() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = immediate(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

void Cpu::and_zp() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = zeropage(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

void Cpu::and_zpx() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = zeropage(value + X) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

void Cpu::and_abs() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address);
  uint8_t result = absolute(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

void Cpu::and_absx() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + X);
  uint8_t result = absolute(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

void Cpu::and_absy() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + Y);
  uint8_t result = absolute(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

void Cpu::and_indx() {
  uint8_t msb = memory.read(PC + X + 2);
  uint8_t lsb = memory.read(PC + X + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address);
  uint8_t result = absolute(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

void Cpu::and_indy() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + Y);
  uint8_t result = absolute(value) & AC;
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x02);
}

// -------------- ASL Shift Left One Bit (Memory or Accumulator) ---------------
// com excessão de asl_acc, que ocorre no acumulador, as operações acontecem
// diretamente no valor contido no end. de memória e o resultado obtido
// é devolvido ao mesmo endereço.
void Cpu::asl_flags_handler(uint8_t value_orig, uint8_t value_new) {
  if (value_new & 0b10000000) {
    setFlag(Flag::N);
  }

  if (value_new == 0) {
    setFlag(Flag::Z);
  }

  if (value_new <= AC) {
    setFlag(Flag::C);
  }
}

void Cpu::asl_acc() {
  uint8_t value = AC;
  uint8_t result = AC << 0x01;
  asl_flags_handler(value, result);
  incrementPC(0x01);
}

void Cpu::asl_zp() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = zeropage(value) << 0x01;
  memory.write(PC + 1, result);
  asl_flags_handler(value, result);
  incrementPC(0x02);
}

void Cpu::asl_zpx() {
  uint8_t value = memory.read(PC + 1);
  uint8_t result = zeropage(value + X) << 0x01;
  memory.write(PC + 1, result);
  asl_flags_handler(value, result);
  incrementPC(0x02);
}

void Cpu::asl_abs() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address);
  uint8_t result = absolute(value) << 0x01;
  memory.write(address, result);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}

void Cpu::asl_absx() {
  uint8_t msb = memory.read(PC + 2);
  uint8_t lsb = memory.read(PC + 1);
  uint16_t address = concat2Bytes(msb, lsb);

  uint8_t value = memory.read(address + X);
  uint8_t result = absolute(value) << 0x01;
  memory.write(address + X, result);
  adc_flags_handler(value, result);
  AC = result;
  incrementPC(0x03);
}
