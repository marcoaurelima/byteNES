
#include "Cpu.hpp"
#include "Gui.hpp"
#include "Memory.hpp"

int main() {

  Memory mem;
  mem.fillZeroData();
  // mem.fillRandomData();
  mem.loadMemoryFromFile("asm/program.bin", 0x1000);

  Cpu cpu(mem);
  cpu.setAsmAddress(0x1000);

  Gui gui(cpu);
  gui.show();

  return 0;
}
