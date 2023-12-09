#include "Memory.hpp"
#include <cstddef>
#include <ctime>
#include <fstream>
#include <iostream>

/*
| Address range | Size  | Device
| $0000–$07FF   | $0800 | 2 KB internal RAM
| $0800–$0FFF   | $0800 |\
| $1000–$17FF   | $0800 | Mirrors of $0000–$07FF
| $1800–$1FFF   | $0800 |/
| $2000–$2007   | $0008 | NES PPU registers
| $2008–$3FFF   | $1FF8 | Mirrors of $2000–$2007 (repeats every 8 bytes)
| $4000–$4017   | $0018 | NES APU and I/O registers
| $4018–$401F   | $0008 | APU and I/O functionality that is normally disabled.
See CPU Test Mode | $4020–$FFFF   | $BFE0 | Cartridge space: PRG ROM, PRG RAM,
and mapper registers
*/

Memory::Memory() {}

Memory::~Memory() {}

// Preenche a memória com dados aleatórios
void Memory::fillRandomData() {
  srand(time(NULL));
  for (auto &i : data) {
    i = rand() % 0x0F;
  }
  saveMemoryStatusToFile();
}

void Memory::fillSequencialData() {
  for (size_t i = 0; i < data.size(); i++) {
    data[i] = i;
  }
  saveMemoryStatusToFile();
}

void Memory::fillZeroData() {
  for (int i = 0; i < MEMSIZE; i++) {
    data[i] = 0;
  }
  saveMemoryStatusToFile();
}

// Função que verifica se um determinado endereço está em um espaço mirrored,
// e então corrige pra um valor de endereço válido.
uint16_t verifyMirroredAddress(uint16_t address) {
  if (address >= 0x1000 || address >= 0x4000) {
    return address;
  }

  if (address >= 0x1000 && address <= 0x1FFF) {
    return (address % 0x1000);
  }

  if (address >= 0x2008 && address <= 0x3FFF) {
    return (address % 0x0008);
  }

  return address;
}

uint8_t Memory::read(uint16_t address) {
  // uint16_t veriAddr = verifyMirroredAddress(address);
  return data[address];
}

void Memory::saveMemoryStatusToFile() {
  std::ofstream file("memory_status/memory.bin", std::ios::trunc);
  if (file.is_open()) {
    for (size_t i = 0; i < data.size(); i++) {
      file << data[i];
    }
  }
}

void Memory::write(uint16_t address, uint8_t value) {
  /*
  | Address range | Size  | Device
  | $0000–$07FF   | $0800 | 2 KB internal RAM
  | $0800–$0FFF   | $0800 |\
  | $1000–$17FF   | $0800 | Mirrors of $0000–$07FF
  | $1800–$1FFF   | $0800 |/
  | $2000–$2007   | $0008 | NES PPU registers
  | $2008–$3FFF   | $1FF8 | Mirrors of $2000–$2007 (repeats every 8 bytes)
  | $4000–$4017   | $0018 | NES APU and I/O registers
  | $4018–$401F   | $0008 | APU and I/O functionality that is normally disabled.
  See CPU Test Mode | $4020–$FFFF   | $BFE0 | Cartridge space: PRG ROM, PRG RAM,
  and mapper registers
  */
  if (address < 0x2000) {
    for (size_t i = 0x00; i < 0x2000; i += 0x0800) {
      data[address + i] = value;
    }
    saveMemoryStatusToFile();
    return;
  }
  if (address >= 0x2000 && address < 0x4000) {
    for (size_t i = 0x00; i < 0x4000; i += 0x0008) {
      data[address + i] = value;
    }
    saveMemoryStatusToFile();
    return;
  }
  data[address] = value;
  saveMemoryStatusToFile();
}

void Memory::loadMemoryFromFile(std::string path, uint16_t addrBegin) {
  std::ifstream file(path, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "Error in open file \"" << path << "\"\n";
    exit(1);
  }

  char byte{};
  uint16_t i = 0;
  while (file.read(&byte, sizeof(char))) {
    write((i + addrBegin), byte);
    ++i;
  }

  filePath = path;
  saveMemoryStatusToFile();
}

void Memory::reset() {
  fillZeroData();
  loadMemoryFromFile(filePath);
  saveMemoryStatusToFile();
}

std::string Memory::getFilePath() { return filePath; }
