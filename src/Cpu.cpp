#include "Cpu.hpp"


Cpu::Cpu(Memory &memory, uint16_t PC, uint8_t SP, uint8_t A, uint8_t X, uint8_t Y, uint8_t SR) 
        : memory(memory), PC(PC), SP(SP), A(A), X(X), Y(Y), SR(SR)
{
}

Cpu::~Cpu()
{
}

uint16_t Cpu::getPC()
{
    return PC;
}

uint8_t Cpu::getSP()
{
    return SP;
}

uint8_t Cpu::getA()
{
    return A;
}

uint8_t Cpu::getX()
{
    return X;
}

uint8_t Cpu::getY()
{
    return Y;
}

uint8_t Cpu::getSR()
{
    return SR;
}

void Cpu::setFlag(Flag flag)
{
    switch (flag)
    {
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

// Concatena 2 variaveis de 8 bits em uma única de 16 bits.
// (msb) Most Significant Byte
// (lsb) Least Significant Byte
uint16_t concat2Bytes(uint8_t msb, uint8_t lsb)
{
    uint16_t result = 0;
    result = (msb << 8);
    result = (result | lsb);
    return result;
}

// -------------- ADC (ADd with Carry) -------------- //

// Adiciona o valor imediato diretamente ao registrador acumulador
void Cpu::adc_im(uint8_t value)
{
    uint8_t result = A + value;

    if (result & 0b10000000)
        setFlag(Flag::N);

    if (result == 0)
        setFlag(Flag::Z);

    if (result <= A)
        setFlag(Flag::C);

    if (((A ^ value) & 0b10000000) && ((A ^ result) & 0b10000000))
        setFlag(Flag::V);

    A = result;
}

// Busca o dado no endereço fornecido no operando (na zero page),
// e em posse desse valor, adicionar ao acumulador.
void Cpu::adc_zp(uint8_t address)
{
    uint8_t value = memory.read(address);
    adc_im(value);
}

// Soma o endereço do operando (na zero page) com o valor contido no registrador X.
// O resultado é o endereço que a CPU deve buscar o dado na memória.
// A cpu então busca esse dado e adiciona ao acomulador.
// Aqui, a flag carry não é afetada na aritmetica de endereços.
void Cpu::adc_zpx(uint8_t address)
{
    uint8_t result = address + X;
    adc_zp(result);
}

// similar ao adc_zp(), so que no caso do absolute, é um endereço de 16 bits,
// pois ele pode acessar não apenas o zero page, mas a memoria ram inteira.
// Busca o dado no endereço fornecido no operando (na memória total),
// e em posse desse valor, adicionar ao acumulador.
void Cpu::adc_abs(uint16_t address)
{
    uint8_t value = memory.read(address);
    adc_im(value);
}

// Soma o endereço do operando (na memoria total) com o valor contido no registrador X.
// O resultado é o endereço que a CPU deve buscar o dado na memória.
// A cpu então busca esse dado e adiciona ao acomulador.
// Aqui, a flag carry não é afetada na aritmetica de endereços.
void Cpu::adc_absx(uint16_t address)
{
    uint16_t result = address + X;
    adc_abs(result);
}

// Soma o endereço do operando (na memoria total) com o valor contido no registrador X.
// O resultado é o endereço que a CPU deve buscar o dado na memória.
// A cpu então busca esse dado e adiciona ao acomulador.
// Aqui, a flag carry não é afetada na aritmetica de endereços.
void Cpu::adc_absy(uint16_t address)
{
    uint16_t result = address + Y;
    adc_abs(result);
}

// Junta o edereço do operando (8 bits) com o valor do registrador X (8 bits)
// transformando-os em um endereço de 16 bits.
// Esta junção é bit a bit, sendo que o valor de X é o byte mais significativo, e
// o valor do operando é o menos significativo.
// O dado é buscado nesse endereço e armazenado no acumulador.
void Cpu::adc_indx(uint8_t address)
{
    uint16_t result = concat2Bytes(X, address);
    adc_abs(result);
}

// Junta o edereço do operando (8 bits) com o valor do registrador Y (8 bits)
// transformando-os em um endereço de 16 bits.
// Esta junção é bit a bit, sendo que o valor de X é o byte mais significativo, e
// o valor do operando é o menos significativo.
// O dado é buscado nesse endereço e armazenado no acumulador.
void Cpu::adc_indy(uint8_t address)
{
    uint16_t result = concat2Bytes(X, address);
    adc_abs(result);
}

// -------------- STX (STore X register) -------------- //

// Armazena o valor contido no registrador X no endereço do operando (zero page).
void Cpu::stx_zp(uint8_t address)
{
}

// Armazena o valor contido no registrador X no endereço do operando, somado com o valor contido no
// registrador Y.
void Cpu::stx_zpy(uint8_t address)
{
}

// Armazena o valor contido no registrador X no endereço do operando de forma absoluta (toda a memória)
void Cpu::stx_abs(uint8_t address)
{
}
