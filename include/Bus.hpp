#ifndef BUS_H
#define BUS_H

#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>

#include "Memory.hpp"
#include "Cpu.hpp"
class Bus
{
public:
    Bus(Cpu& cpu, Memory& memory);
    ~Bus();

    // Carrega dados presentes em um arquivo (path) diretamente em Memory&, comoeçando 
    // pelo endereço especificadop em addrBegin.
    void loadMemoryFromFile(std::string path, uint16_t addrBegin);
private:
    Cpu& cpu;
    Memory& memory;
};

#endif
