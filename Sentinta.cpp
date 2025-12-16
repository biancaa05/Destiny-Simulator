#include "Sentinta.h"
#include <iostream>
#include <iomanip>

void Sentinta::afiseazaDetalii() const {
    std::cout << "-------------------------------------------\n";
    std::cout << "             SENTINTA EMISA \n";
    std::cout << "-------------------------------------------\n";
    std::cout << "Durata Inchisoare: " << durataInchisoareAni << " ani\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Amenda: " << amendaValoare << " RON\n";
    std::cout << "Impact Cazier: +" << impactCazierFinal << std::endl;
    std::cout << "-------------------------------------------\n";
}