#include "Aventura.h"
#include "Statistici.h"

void DrumetieMontana::aplicaImpact(Statistici& stats) {
    stats.modificaStatistica("Inteligenta", 15);
    stats.modificaStatistica("Sanatate", -5);
}
void DrumetieMontana::afiseaza(std::ostream& os) const {
    os << "Sanatate -5, Inteligenta +15.";
}

void Salvare::aplicaImpact(Statistici& stats) {
    stats.modificaStatistica("Sanatate", -10);
    stats.modificaStatistica("Fericire", 25);
}
void Salvare::afiseaza(std::ostream& os) const {
    os << "Sanatate -10, Fericire +25.";
}
void Salvare::oferaMotivatieExtra(Statistici& stats) {
    stats.modificaStatistica("Fericire", 5);
}

void Vanatoare::aplicaImpact(Statistici& stats) {
    stats.modificaStatistica("Sanatate", -30);
    stats.modificaStatistica("Aspect", -20);
    stats.modificaStatistica("Fericire", -10);
}
void Vanatoare::afiseaza(std::ostream& os) const {
    os << "Sanatate -30, Aspect -20, Fericire -10.";
}