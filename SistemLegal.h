#ifndef SISTEM_LEGAL_H
#define SISTEM_LEGAL_H

#include "Crima.h"
#include "Personaj.h"

class SistemLegal {
public:
    SistemLegal() = default;
    SistemLegal(const SistemLegal&) = delete;
    SistemLegal& operator=(const SistemLegal&) = delete;

    static SistemLegal& getInstance() {
        static SistemLegal instance;
        return instance;
    }

    static bool verificaPrindere(const Personaj& p, const Crima& c) ;
    static void emiteSentinta(Personaj& p, const Crima& c) ;
};

#endif // SISTEM_LEGAL_H