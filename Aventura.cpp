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

void NouHobby::aplicaImpact(Statistici& stats) {

    stats.modificaStatistica("Sanatate", 2);
    stats.modificaStatistica("Inteligenta", 2);

    if (const int sansaFinala = getRandomInt(1, 100); sansaFinala <= 10) {
        stats.modificaStatistica("Fericire", 20);
        stats.modificaStatistica("Sanatate", 10);
        stats.modificaBani(50.0);
        std::cout << "\n[EXCELENȚĂ] Ai excelat! Fericire +20, Sanatate +10, si o Bursa de 50K!";
    } else if (sansaFinala <= 40) {
        // 2. EȘEC (Frustrare)
        stats.modificaStatistica("Fericire", -5);
        stats.modificaStatistica("Sanatate", -2);
        std::cout << " [ESEC] Nu te-ai descurcat. Fericire -5, Sanatate -2.";
    } else {
        // 3. SUCCES MODERAT (Restul, 50% șansă)
        stats.modificaStatistica("Fericire", 10);
        stats.modificaStatistica("Sanatate", 5);
        std::cout << " [SUCCES] Ai progresat! Fericire +10, Sanatate +5.";
    }
}

void NouHobby::afiseaza(std::ostream& os) const {
    os << "Impact Baza: Sanatate +2, Inteligenta +2. Risc Esec (pierdere): 40%. Sansa Excelenta (bonus bani): 10%.";
}