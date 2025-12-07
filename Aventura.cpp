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
        std::cout << "\n[EXCELENTA] Ai excelat! Fericire +20, Sanatate +10, si o Bursa de 50K!";
    } else if (sansaFinala <= 40) {
        stats.modificaStatistica("Fericire", -5);
        stats.modificaStatistica("Sanatate", -2);
        std::cout << " [ESEC] Nu te-ai descurcat. Fericire -5, Sanatate -2.";
    } else {
        stats.modificaStatistica("Fericire", 10);
        stats.modificaStatistica("Sanatate", 5);
        std::cout << " [SUCCES] Ai progresat! Fericire +10, Sanatate +5.";
    }
}

void NouHobby::afiseaza(std::ostream& os) const {
    os << "Impact Baza: Sanatate +2, Inteligenta +2. Risc Esec (pierdere): 40%. Sansa Excelenta (bonus bani): 10%.";
}

void EvenimentSocial::aplicaImpact(Statistici& stats) {
    stats.modificaStatistica("Aspect", 3);
    stats.modificaBani(-5.0);

    const int aspectScore = stats.getAspect().getValoare();
    const int sansaSuccesAjustata = 75 + (aspectScore / 10);

    std::cout << "\n[SOCIAL] Cost: -5K. Aspect +3. Sansa de Succes Social Ajustata: " << sansaSuccesAjustata << "%.";

    if (getRandomInt(1, 100) <= sansaSuccesAjustata) {
        stats.modificaStatistica("Fericire", 15);
        stats.modificaStatistica("Aspect", 10);
        std::cout << " [SUCCES!] Ai impresionat! Fericire +15, Aspect +10.";
    } else {
        stats.modificaStatistica("Fericire", -15);
        std::cout << " [ESEC] Te-ai facut de rusine. Fericire -15.";
    }

}
void EvenimentSocial::afiseaza(std::ostream& os) const {
    os << "Cost: -5K. Impact Baza: Aspect +3. Risc de Esec (Rusine): 25%. Succesul aduce +15 Fericire, +10 Aspect.";
}

void ScandalPublic::aplicaImpact(Statistici& stats) {
    constexpr double COST_LEGAL = 20.0;
    stats.modificaBani(-COST_LEGAL);

    const int inteligenta = stats.getInteligenta().getValoare();
    const int sansaEsecAjustata = 60 - (inteligenta / 5);
    stats.modificaStatistica("Fericire", -10);

    std::cout << "\n[SCANDAL] Cost Legal: -" << COST_LEGAL << "K. Esec Baza: 60%. Sansa de Esec Ajustata: " << sansaEsecAjustata << "%.";

    if (getRandomInt(1, 100) <= sansaEsecAjustata) {
        stats.modificaStatistica("Fericire", -35);
        stats.modificaStatistica("Inteligenta", -10);
        std::cout << " [ESEC!] Criza gestionata prost. Fericire -35, Inteligenta -10.";
    } else {
        stats.modificaStatistica("Fericire", 5);
        std::cout << " [SUCCES!] Criza gestionata eficient. Pierdere minima de Fericire (-5 de bază).";
    }
}
[[nodiscard]] int Aventura::getSansaEsecBaza() const {
    return this->sansaEsecBaza;
}
void ScandalPublic::afiseaza(std::ostream& os) const {
    os << "Cost Bază: -20K. Risc Esec (pierderi grave): 60% (influentat de Inteligenta).";
}