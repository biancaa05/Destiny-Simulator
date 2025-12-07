#include "Cariera.h"

void Cariera::ajusteazaSatisfactia(const int val) {
    satisfactie = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, val));
}

Cariera::Cariera() {
    this->numeJob = "Somaj";
    this->salariuAnual = 0;
    this->satisfactie = 10;
    this->cerintaInteligenta = 0;
}
Cariera::Cariera(const std::string& nume, const int salariu, const int satisf, const int cerinta) {
    this->numeJob = nume;
    this->salariuAnual = salariu;
    this->satisfactie = satisf;
    this->cerintaInteligenta = cerinta;
}

[[nodiscard]] const std::string& Cariera::getNumeJob() const { return numeJob; }
[[nodiscard]] int Cariera::getSalariuAnual() const { return salariuAnual; }


int Cariera::incearcaPromovare(const int inteligentaPersonaj) {
    const int sansaBaza = (inteligentaPersonaj - cerintaInteligenta) / 2;
    const int sansaFinala = std::max(10, std::min(90, sansaBaza + getRandomInt(-10, 10)));
    std::cout << "* PROMOVARE: Sansa de succes: " << sansaFinala << "%. ";
    if (getRandomInt(1, 100) <= sansaFinala) {
        salariuAnual += 20;
        satisfactie += 15;
        ajusteazaSatisfactia(satisfactie);
        std::cout << "SUCCES! Salariu: +20, Satisfactie: +15." << std::endl;
        return 1;
    }
    else {
        std::cout << "ESEC. Nu esti inca pregatit." << std::endl;
        return 0;
    }
}
std::ostream& operator<<(std::ostream& os, const Cariera& c) {
    os << "Job: " << c.numeJob << " | Salariu: " << c.salariuAnual << "K | Satisfactie: " << c.satisfactie << "%";
    return os;
}