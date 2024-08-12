
#include "Cpu.hpp"
#include "Gui.hpp"
#include "Mem.hpp"

int main() {

  Memory mem;
  mem.fillZeroData();
  // mem.fillRandomData();
  mem.enableSaveStatusToFile(false);
  mem.loadMemoryFromFile("asm/program.bin", 0x0600);

  Cpu cpu(mem);
  cpu.setAsmAddress(0x0600);
  cpu.setInternalClockValue(2 * MHz);
  cpu.start();

  /*Gui gui(cpu);*/
  /*gui.show();*/

  return 0;
}
