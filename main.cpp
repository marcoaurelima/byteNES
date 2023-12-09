
#include "Cpu.hpp"
#include "Gui.hpp"
#include "Memory.hpp"

int main() {

  Memory mem;
  mem.fillZeroData();
  // mem.fillRandomData();
  mem.loadMemoryFromFile("asm/program.bin");

  Cpu cpu(mem);

  Gui gui(cpu);
  gui.show();

  return 0;
}
