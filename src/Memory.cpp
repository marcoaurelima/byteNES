#include "Memory.hpp"

/*
| Address range | Size  | Device
| $0000–$07FF   | $0800 | 2 KB internal RAM
| $0800–$0FFF   | $0800 |\ 
| $1000–$17FF   | $0800 | Mirrors of $0000–$07FF
| $1800–$1FFF   | $0800 |/
| $2000–$2007   | $0008 | NES PPU registers
| $2008–$3FFF   | $1FF8 | Mirrors of $2000–$2007 (repeats every 8 bytes)
| $4000–$4017   | $0018 | NES APU and I/O registers
| $4018–$401F   | $0008 | APU and I/O functionality that is normally disabled. See CPU Test Mode
| $4020–$FFFF   | $BFE0 | Cartridge space: PRG ROM, PRG RAM, and mapper registers
*/

Memory::Memory()
{
    for (int i = 0; i < MEMSIZE; i++)
    {
        data[i] = 0;
    }
}

Memory::~Memory()
{
}

// Função que verifica se um determinado endereço está em um espaço mirrored,
// e então corrige pra um valor de endereço válido.
uint16_t verifyMirroredAddress(uint16_t address)
{
    if(address >= 0x1000 || address >= 0x4000)
    {
        return address;
    }

    if(address >= 0x1000 && address <= 0x1FFF)
    {
        return (address % 0x1000);
    }
    
    if(address >= 0x2008 && address <= 0x3FFF)
    {
        return (address % 0x0008);
    }
    
    return address;
}

uint8_t Memory::read(uint16_t address)
{
    uint16_t veriAddr = verifyMirroredAddress(address);
    return data[veriAddr];
}
void Memory::write(uint16_t address, uint8_t value)
{
    uint16_t veriAddr = verifyMirroredAddress(address);
    data[veriAddr] = value;
}
