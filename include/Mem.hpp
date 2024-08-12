#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <string>

#define MEMSIZE 0xFFFF + 0x0001

class Memory {
public:
  Memory();
  ~Memory();

  uint8_t read(uint16_t address);
  void write(uint16_t address, uint8_t value);

  void fillRandomData();
  void fillSequencialData();
  void fillZeroData();

  void loadMemoryFromFile(std::string path, uint16_t addrBase = 0x00);
  void enableSaveStatusToFile(bool enable);
  void saveMemoryStatusToFile();
  std::string getFilePath();
  void reset();

private:
  // Habilita o salvamento do status da memória do emulador em um
  // arquivo externo (memory_status.bi). Habilitar apenas para debugar
  // pois as operaçṍes de IO causa overhead e lentidão na execução dos 
  // opcodes
  bool saveStatusToFile;
  std::array<uint8_t, MEMSIZE> data;
  std::string filePath;
  uint16_t asmAddress;
};

#endif
