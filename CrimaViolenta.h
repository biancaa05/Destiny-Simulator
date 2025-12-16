#ifndef CRIMAVIOLENTA_H
#define CRIMAVIOLENTA_H

#include "Crima.h"

class CrimaViolenta final : public Crima {
private:
    double impactSanatateNegativ;

public:
    CrimaViolenta(const std::string& nume, double risc, double minBani, double maxBani, int cazier, double impactSanatate);
    bool executa(Personaj& p) const override;
};

#endif // CRIMAVIOLENTA_H