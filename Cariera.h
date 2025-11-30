#ifndef CARIERA_H
#define CARIERA_H

#include "sim_common.h"

class Cariera {
private:
    std::string numeJob;
    int salariuAnual;
    int satisfactie;
    int cerintaInteligenta;

    void ajusteazaSatisfactia(int val);

public:
    Cariera();
    Cariera(const std::string& nume, int salariu, int satisf, int cerinta);

    [[nodiscard]] const std::string& getNumeJob() const;
    [[nodiscard]] int getSalariuAnual() const;

    int incearcaPromovare(int inteligentaPersonaj);
    friend std::ostream& operator<<(std::ostream& os, const Cariera& c);
};

#endif // CARIERA_H