
#include "Cpu.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
  
  Cpu cpu;


  Gui gui(cpu);
  gui.show();

  return 0;
}
