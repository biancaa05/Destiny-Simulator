#include "Vacanta.h"
#include "GeneratorRandom.h"
#include "SimUtilities.h"
#include <iostream>

void Vacanta::selecteazaDestinatieAleatorie() {
    destinatieAleasa = alegeElementAleatoriu(DESTINATII_VACANTA);
}

Vacanta::Vacanta() {
    selecteazaDestinatieAleatorie();

    const std::vector<std::string> tipuriPartener = {"Mama", "Tata", "Prieten", "Coleg"};
    
    const int nrParteneri = GeneratorRandom::getInstance().getRandomInt(1, 2);
    
    for (int i = 0; i < nrParteneri; ++i) {
        const std::string partener = alegeElementAleatoriu(tipuriPartener);
        parteneriVacanta.push_back(partener);
    }
}

void Vacanta::organizeaza(Statistici& stats, std::vector<Relatie>& relatii) const {
    const double cost = destinatieAleasa.costBaza;
    const int bonusFericire = destinatieAleasa.bonusFericire;

    if (stats.getBani() < cost) {
        std::cout << "[VACANTA] Nu ai suficienti bani (" << stats.getBani() 
                  << "K) pentru vacanta la " << destinatieAleasa.nume 
                  << " (Cost: " << cost << "K)." << std::endl;
        return;
    }

    stats.modificaBani(-cost);
    stats.modificaStatistica("Fericire", bonusFericire);

    std::cout << "\n[VACANTA] Ai plecat in vacanta la " << destinatieAleasa.nume 
              << " cu un cost de " << cost << "K." << std::endl;
    std::cout << "  -> Fericire +" << bonusFericire << "." << std::endl;

    int relatiiModificate = 0;
    
    for (const std::string& tipPartener : parteneriVacanta) {
        for (Relatie& r : relatii) {
            if (r.getTipRelatie() == tipPartener) {
                constexpr int bonusRelatie = 10;
                r.imbunatatesteRelatia(bonusRelatie);
                
                std::cout << "  -> Relatia cu " << r.getNumePersoana() 
                          << " (" << r.getTipRelatie() << ") a crescut cu +" 
                          << bonusRelatie << " Afectiune." << std::endl;
                relatiiModificate++;
                break;
            }
        }
    }
    
    if (relatiiModificate == 0) {
        std::cout << "  -> Nu au fost gasite relatii active de tipul " 
                  << parteneriVacanta[0] << " pentru a beneficia de bonus." << std::endl;
    }
}
