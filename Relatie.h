#ifndef RELATIE_H
#define RELATIE_H

#include "sim_common.h"

class Relatie {
    std::string numePersoana;
    std::string tipRelatie;
    int nivelAfectiune;

public:
    Relatie(const std::string& nume, const std::string& tip, int afectiune);
    Relatie(const Relatie& other);
    Relatie& operator=(const Relatie& other);
    ~Relatie()=default;

    [[nodiscard]] int getNivelAfectiune() const;
    [[nodiscard]] const std::string& getNumePersoana() const;
    [[nodiscard]] const std::string& getTipRelatie() const;

    void imbunatatesteRelatia(int puncte);

    friend std::ostream& operator<<(std::ostream& os, const Relatie& r);
};

#endif // RELATIE_H