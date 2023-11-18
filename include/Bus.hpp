#ifndef BUS_H
#define BUS_H

#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>

#include "Memory.hpp"
class Bus
{
public:
    Bus();
    ~Bus();

    // Carrega dados presentes em um arquivo (path) diretamente em Memory&, comoeçando 
    // pelo endereço especificadop em addrBegin.
    void loadROMToMemory(Memory& memory, std::string path, uint16_t addrBegin);
private:

};

#endif
