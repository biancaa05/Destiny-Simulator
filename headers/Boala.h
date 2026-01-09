// Boala.h

#ifndef BOALA_H
#define BOALA_H

#include <string>

class Personaj;

class Boala {
private:
    std::string nume;
    double impactSanatateAnual;
    double costTratamentAnualK;
    double riscDecesAnual;

public:
    Boala(std::string  nume, double impact, double cost, double risc);

    [[nodiscard]] const std::string& getNume() const;
    [[nodiscard]] double getCostTratamentAnual() const;

    void aplicaEfectAnual(Personaj& p) const;
};

#endif // BOALA_H