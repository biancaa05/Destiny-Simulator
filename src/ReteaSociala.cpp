#include "../headers/ReteaSociala.h"
#include "../headers/GeneratorRandom.h"
#include "../headers/Statistici.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const ReteaSociala& r) {
    os << "Platforma: " << r.platforma;
    return os;
}

void ReteaSociala::posteazaCeva(Statistici& stats, std::vector<Relatie>& relatii) const {

    const int aspect = stats.getAspect().getValoare();
    const int vizibilitateInitiala = stats.getVizibilitate().getValoare();
    const int fericire = stats.getFericire().getValoare();

    const int ajustareFericire = (100 - fericire) / 20;
    const int sansaControversa = 10 + ajustareFericire;

    const int ajustarePopularitate = (vizibilitateInitiala + aspect) / 40;
    const int sansaViral = 10 + ajustarePopularitate;

    const int randomRoll = GeneratorRandom::getInstance().getRandomInt(1, 100);

    int bonusVizibilitate = 0;
    int bonusFericire = 0;

    std::cout << "\n[SOCIAL MEDIA - " << platforma << "] Postezi ceva...";

    if (randomRoll <= sansaControversa) {
        bonusVizibilitate = 40;
        bonusFericire = -20;

        int relatiiStricate = 0;
        for (Relatie& r : relatii) {
            if (GeneratorRandom::getInstance().getRandomInt(1, 100) <= 60) {
                r.imbunatatesteRelatia(-10);
                relatiiStricate++;
            }
        }
        std::cout << " CONTROVERSA! Ai castigat Vizibilitate (+" << bonusVizibilitate << "), dar ai pierdut " << relatiiStricate << " urmaritori.";

    }

    else if (randomRoll <= (sansaControversa + sansaViral)) {
        bonusVizibilitate = 30;
        bonusFericire = 15;
        std::cout << " SUCCES! Postarea a devenit VIRALA! (Vizibilitate +" << bonusVizibilitate << ")";

    } else {
        bonusVizibilitate = (aspect / 30) + 1;
        bonusFericire = 2;
        std::cout << " Postarea este normala (Vizibilitate +" << bonusVizibilitate << ").";
    }

    stats.modificaStatistica("Vizibilitate", bonusVizibilitate);
    stats.modificaStatistica("Fericire", bonusFericire);
}