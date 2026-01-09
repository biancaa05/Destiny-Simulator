#include "../headers/CrimaViolenta.h"
#include "../headers/GeneratorRandom.h"
#include "../headers/SistemLegal.h"

CrimaViolenta::CrimaViolenta(const std::string& nume, const double risc, const double minBani, const double maxBani, const int cazier, const double impactSanatate)
    : Crima(nume, risc, minBani, maxBani, cazier), impactSanatateNegativ(impactSanatate) {}

bool CrimaViolenta::executa(Personaj& p) const {
    std::cout << "\n[ACTIUNE] Te pregatesti pentru: " << nume << " (Foarte Riscant!)\n";

    p.modificaStatistica("Sanatate", impactSanatateNegativ);
    p.modificaStatistica("Fericire", -15);

    if (SistemLegal::getInstance().verificaPrindere(p, *this)) {
        std::cout << "  [CAPTURAT!] Planul a esuat. Confruntare violenta! Urmeaza Sentinta...\n";
        p.adaugaEveniment(p.getVarsta(), nume, "CAPTURAT! Consecinte severe.");
        SistemLegal::getInstance().emiteSentinta(p, *this);
        return false;
    }
    const double castig = GeneratorRandom::getInstance().getDouble(recompensaBaniMin, recompensaBaniMax);
    p.modificaStatistica("Bani", castig);

    std::cout << "  [SUCCES TOTAL] Ai scapat nevatamat! Castig Masiv: " << castig << " K.\n";
    p.adaugaEveniment(p.getVarsta(), nume, "SUCCES. Castig: " + std::to_string(castig));
    return true;
}
