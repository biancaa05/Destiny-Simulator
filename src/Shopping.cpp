#include "../headers/Shopping.h"

void CumparaCasa::aplicaImpact(Statistici& stats) const {
    stats.modificaStatistica("Fericire", 30);
}

void SesiuneSpa::aplicaImpact(Statistici& stats) const {
    stats.modificaStatistica("Fericire", 5);
    stats.modificaStatistica("Aspect", 3);
}

void CumparaCarti::aplicaImpact(Statistici& stats) const {
    stats.modificaStatistica("Inteligenta", 5);
}

void CumparaMasina::aplicaImpact(Statistici &stats) const {
    stats.modificaStatistica("Fericire", 20);
}

void CumparaMedicamente::aplicaImpact(Statistici &stats) const {
    stats.modificaStatistica("Sanatate", 5);
}
