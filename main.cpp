
#include "Bus.hpp"
#include "Memory.hpp"
#include "Cpu.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
  
  Memory mem;
  mem.fillZeroData();

  Cpu cpu(mem);

  Bus bus(cpu, mem);
  bus.loadMemoryFromFile("asm/program.bin");

  Gui gui(cpu);
  gui.show();

  return 0;
}
