#include <iostream>
#include <cstdint>
#include <bitset>

#include "Cpu.h"

uint16_t concat2Bytest(uint8_t msb, uint8_t lsb)
{
    uint16_t result = 0;
    result = (msb << 8);
    result = (result | lsb);
    return result;
}

int main(int argc, char *argv[])
{
  uint8_t msb = 0x12, lsb = 0x34;

  uint16_t sum = concat2Bytest(0x12, 0x34);

  std::cout << std::bitset<8>(msb) << std::endl;
  std::cout << std::bitset<8>(lsb) << std::endl;
  std::cout << std::bitset<16>(sum) << std::endl;

  // Cpu cpu;

  // cpu.printInternals();
  // cpu.adc_im(0x01);
  // cpu.printInternals();
 
  return 0;
}
