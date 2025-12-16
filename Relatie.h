// RELATIE.H (Versiune Corectată și Completă)

#ifndef RELATIE_H
#define RELATIE_H

#include "sim_common.h"

class Relatie {
    std::string numePersoana;
    std::string tipRelatie;
    int nivelAfectiune = 0;
    int varsta = 0;

public:
    Relatie() = default;
    Relatie(const std::string& nume, const std::string& tip, int afectiune, int varstaInit);
    Relatie(const Relatie& other);
    Relatie& operator=(const Relatie& other);
    ~Relatie() = default;

    [[nodiscard]] int getNivelAfectiune() const;
    [[nodiscard]] const std::string& getNumePersoana() const;
    [[nodiscard]] const std::string& getTipRelatie() const;
    [[nodiscard]] int getVarsta() const;

    void setVarsta(int v);

    void imbunatatesteRelatia(int puncte);

    friend std::ostream& operator<<(std::ostream& os, const Relatie& r);
};

#endif // RELATIE_H