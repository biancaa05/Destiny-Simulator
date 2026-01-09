#ifndef CRIMASIMPLA_H
#define CRIMASIMPLA_H

#include "Crima.h"

class CrimaSimpla final : public Crima {
public:
    CrimaSimpla(const std::string& nume, double risc, double minBani, double maxBani, int cazier);
    bool executa(Personaj& p) const override;
};

#endif // CRIMASIMPLA_H