
#include "Bus.hpp"
#include "Memory.hpp"
#include "Cpu.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
  
  Memory mem;

  Cpu cpu(mem);

  Bus bus;
  bus.loadROMToMemory(mem, "asm/program.bin", 0x00);

  Gui gui(cpu);
  gui.show();

  return 0;
}
