#include <iostream>
#include <cstdint>

#include "Cpu.h"


int main(int argc, char *argv[])
{
  Cpu cpu;

  cpu.printInternals();
  cpu.adc_im(0x01);
  cpu.printInternals();
 
  return 0;
}
