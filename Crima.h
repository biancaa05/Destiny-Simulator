#ifndef CRIMA_H
#define CRIMA_H

#include <string>
#include <utility>
#include "Personaj.h"

class Crima {
protected:
    std::string nume;
    double riscProcentual;
    double recompensaBaniMin;
    double recompensaBaniMax;
    int impactCazier;

public:
    Crima(std::string  nume, const double risc, const double minBani, const double maxBani, const int cazier)
        : nume(std::move(nume)), riscProcentual(risc), recompensaBaniMin(minBani), recompensaBaniMax(maxBani), impactCazier(cazier) {}

    virtual ~Crima() = default;

    virtual bool executa(Personaj& p) const = 0;

    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] double getRisc() const { return riscProcentual; }
    [[nodiscard]] int getImpactCazier() const { return impactCazier; }
    [[nodiscard]] double getRecompensaMax() const { return recompensaBaniMax; }
    static const Crima* getCrimaPeDecizie(int id_decizie);
};

#endif // CRIMA_H