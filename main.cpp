
#include "Memory.hpp"
#include "Cpu.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
  
  Memory mem;
  mem.fillZeroData();
  mem.loadMemoryFromFile("asm/program.bin");

  Cpu cpu(mem);

  Gui gui(cpu);
  gui.show();

  return 0;
}
