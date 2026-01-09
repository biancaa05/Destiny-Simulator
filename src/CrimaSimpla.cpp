#include "../headers/CrimaSimpla.h"
#include "../headers/GeneratorRandom.h"
#include "../headers/SistemLegal.h"

CrimaSimpla::CrimaSimpla(const std::string& nume, const double risc, const double minBani, const double maxBani, const int cazier)
    : Crima(nume, risc, minBani, maxBani, cazier) {}

bool CrimaSimpla::executa(Personaj& p) const {
    std::cout << "\n[ACTIUNE] Incepi: " << nume << "...\n";
    
    if (SistemLegal::getInstance().verificaPrindere(p, *this)) {
        std::cout << "  [!!!] Ai fost prins! Urmeaza procesul legal...\n";
        p.adaugaEveniment(p.getVarsta(), nume, "PRINS. Vezi Sentinta.");
        SistemLegal::getInstance().emiteSentinta(p, *this);
        return false;
    } else {
        const double castig = GeneratorRandom::getInstance().getDouble(recompensaBaniMin, recompensaBaniMax);
        p.modificaStatistica("Bani", castig);
        p.modificaStatistica("Fericire", -5); 

        std::cout << "  [OK] Ai reusit! Castig: " << castig << " RON. Riscul a meritat.\n";
        p.adaugaEveniment(p.getVarsta(), nume, "Succes. Castig: " + std::to_string(castig));
        return true;
    }
}