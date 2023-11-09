#include "Cpu.h"

Cpu::Cpu()
{
    ram = new Ram(2048);
    PC = 0;
    SP = 0;
    A = 0xFF;
    Y = 0;
    P = 0;
}

Cpu::~Cpu()
{
}

void Cpu::printInternals()
{
    std::cout << "-------------------------------------------\n";
    std::cout << "   RG\tUSIG\tSIG\tBIN\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "   PC\t" << (int)PC << "\t" << (int)((char)PC) << "\t" << std::bitset<16>(PC) << "\n";
    std::cout << "   SP\t" << (int)SP << "\t" << (int)((char)SP) << "\t" << std::bitset<8>(SP) << "\n";
    std::cout << "   A\t" << (int)A << "\t" << (int)((char)A) << "\t" << std::bitset<8>(A) << "\n";
    std::cout << "   Y\t" << (int)Y << "\t" << (int)((char)Y) << "\t" << std::bitset<8>(Y) << "\n";
    std::cout << "   Y\t" << (int)X << "\t" << (int)((char)X) << "\t" << std::bitset<8>(X) << "\n";
    std::cout << "   P\t" << (int)P << "\t" << (int)((char)P) << "\t" << std::bitset<8>(P) << "\n";
    std::cout << "   \t\t\tNV-BDIZC\n";
    std::cout << "-------------------------------------------\n";
}


void Cpu::setFlag(Flag flag)
{
    switch (flag)
    {
    case Flag::N:
        P = P | 0b10000000;
        break;
    case Flag::V:
        P = P | 0b01000000;
        break;
    case Flag::B:
        P = P | 0b00010000;
        break;
    case Flag::D:
        P = P | 0b00001000;
        break;
    case Flag::I:
        P = P | 0b00000100;
        break;
    case Flag::Z:
        P = P | 0b00000010;
        break;
    case Flag::C:
        P = P | 0b00000001;
        break;
    default:
        break;
    }
}

// Adiciona o valor imediato diretamente ao registrador acumulador
void Cpu::adc_im(uint8_t value)
{
    uint8_t result = A + value;

    if(result & 0b10000000)
        setFlag(Flag::N);

    if (result == 0)
        setFlag(Flag::Z);

    if (result <= A)
        setFlag(Flag::C);

    if(((A ^ value) & 0b10000000) && ((A ^ result) & 0b10000000))
        setFlag(Flag::V);

    A = result;
}

// Busca o dado no endereço fornecido no operando (na zero page), 
// e em posse desse valor, adicionar ao acumulador.
void Cpu::adc_zp(uint8_t address)
{
    uint8_t value = ram->get(address);
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
    uint8_t value = ram->get(address);
    adc_im(value);
}

void Cpu::adc_absx()
{
}
void Cpu::adc_absy()
{
}
void Cpu::adc_indx()
{
}
void Cpu::adc_indy()
{
}