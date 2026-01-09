#include "../headers/Boala.h"
#include "../headers/Personaj.h"
#include "../headers/GeneratorRandom.h"
#include <iostream>
#include <utility>

Boala::Boala(std::string  nume, const double impact, const double cost, const double risc)
    : nume(std::move(nume)), impactSanatateAnual(impact), costTratamentAnualK(cost), riscDecesAnual(risc) {}

const std::string& Boala::getNume() const {
    return nume;
}

double Boala::getCostTratamentAnual() const {
    return costTratamentAnualK;
}

void Boala::aplicaEfectAnual(Personaj& p) const {
    p.modificaStatistica("Sanatate", impactSanatateAnual);

    p.modificaStatistica("Bani", -costTratamentAnualK);

    std::cout << "  - [BOALA] " << nume << " aplica efecte: Sanatate "
              << impactSanatateAnual << ", Cost Tratament -" << costTratamentAnualK << "K." << std::endl;

    if (GeneratorRandom::getInstance().getDouble(0.0, 1.0) < riscDecesAnual) {
        std::cout << "\n--- TRAGEDIE! ---" << std::endl;
        std::cout << p.getNumeComplet() << " a decedat din cauza complicatiilor de " << nume << ".\n";
        p.setEsteMort(true);
    }
}