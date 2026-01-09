#ifndef VACANTA_H
#define VACANTA_H

#include "Statistici.h"
#include "Relatie.h"
#include "sim_common.h"
#include <string>
#include <vector>

class Vacanta {
private:
    DestinatieVacanta destinatieAleasa{};
    std::vector<std::string> parteneriVacanta;

    void selecteazaDestinatieAleatorie();

public:
    Vacanta();

    void organizeaza(Statistici& stats, std::vector<Relatie>& relatii) const;

    [[nodiscard]] const DestinatieVacanta& getDestinatie() const { return destinatieAleasa; }
};

#endif // VACANTA_H