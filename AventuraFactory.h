#ifndef AVENTURAFACTORY_H
#define AVENTURAFACTORY_H

#include <memory>
#include <stdexcept>

#include "Aventura.h"
#include "GeneratorRandom.h"

class AventuraFactory {
public:
    AventuraFactory() = delete;
    AventuraFactory(const AventuraFactory&) = delete;
    AventuraFactory& operator=(const AventuraFactory&) = delete;
    static std::unique_ptr<Aventura> creeazaAventuraAleatoare() {
        switch (GeneratorRandom::getInstance().getRandomInt(1, 7)) {
            case 1: return std::make_unique<DrumetieMontana>();
            case 2: return std::make_unique<Salvare>();
            case 3: return std::make_unique<Vanatoare>();
            case 4: return std::make_unique<NouHobby>();
            case 5: return std::make_unique<EvenimentSocial>();
            case 6: return std::make_unique<ScandalPublic>();
            case 7: return std::make_unique<MostenireNeasteptata>();
            default:
                throw std::runtime_error("Eroare Factory: Tip de aventura in afara intervalului [1-7].");
        }
    }
};

#endif // AVENTURAFACTORY_H