#include "SistemLegal.h"
#include "GeneratorRandom.h"
#include <cmath>
#include <iostream>
#include "Sentinta.h"
#include "Personaj.h"

bool SistemLegal::verificaPrindere(const Personaj& p, const Crima& c) {
    double sansaDePrindere = c.getRisc();

const auto inteligenta = static_cast<double>(p.getStatistici().getInteligenta().getValoare());
    sansaDePrindere -= (inteligenta / 100.0) * 0.20;

    if (sansaDePrindere < 0.05) sansaDePrindere = 0.05;

    return GeneratorRandom::getInstance().getChance(sansaDePrindere);
}

void SistemLegal::emiteSentinta(Personaj& p, const Crima& c) {

    const int durataMin = static_cast<int>(std::round(c.getImpactCazier() / 5.0));
    const int durataMax = static_cast<int>(std::round(c.getImpactCazier() / 2.0));

    int aniInchisoare = GeneratorRandom::getInstance().getRandomInt(durataMin, durataMax);
    const double amenda = GeneratorRandom::getInstance().getDouble(c.getRecompensaMax() * 0.5, c.getRecompensaMax() * 2.0);
    const int impactCazierFinal = c.getImpactCazier();

    const double baniPersonaj = p.getStatistici().getBani();

    if (constexpr double costAvocat = 50000.0; baniPersonaj >= costAvocat && c.getImpactCazier() >= 20) {
        std::cout << "  [PROCES] Personajul plateste un avocat scump. Șanse mai bune!\n";
        p.modificaStatistica("Bani", -costAvocat); 
        aniInchisoare = static_cast<int>(std::round(aniInchisoare * 0.7));
    } else {
        std::cout << "  [PROCES] Avocat din oficiu. Sentinta ramane la riscul maxim.\n";
        aniInchisoare = static_cast<int>(std::round(aniInchisoare * 1.2));
    }
    
    if (aniInchisoare < 1) aniInchisoare = 1;
    if (aniInchisoare > 30) aniInchisoare = 30;

    const Sentinta sentinta(aniInchisoare, amenda, impactCazierFinal);
    sentinta.afiseazaDetalii();
    
    p.aplicaSentinta(sentinta);
}