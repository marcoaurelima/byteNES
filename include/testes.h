#ifndef TESTES_H
#define TESTES_H
#include <iostream>
#include "Cpu.h"

// Teste básico - ADC com números positivos:
void test_ADC1()
{
    // Carrega o valor 10 no registrador A
    Cpu cpu(0,0,10,0,0);
    cpu.showStatus();

    //Adiciona 5 a A
    cpu.adc_im(5);

    cpu.showStatus();
}

#endif