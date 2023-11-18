
#include "Bus.hpp"
#include "Memory.hpp"
#include "Cpu.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
  


  Memory mem;
  // mem.write(0x00, 11);

  Cpu cpu(mem);

  Bus bus;
  bus.loadROMToMemory(mem, "rom/rom.bin", 0x00);

  Gui gui(cpu);
  gui.show();

  return 0;
}
