#include "../headers/Statistici.h"
#include "../headers/GeneratorRandom.h"
#include "../headers/StatisticaLimita.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "../headers/sim_common.h"

Statistici::Statistici() :
    sanatate(GeneratorRandom::getInstance().getRandomInt(30, 90), 100),
    fericire(GeneratorRandom::getInstance().getRandomInt(30, 90), 100),
    inteligenta(GeneratorRandom::getInstance().getRandomInt(30, 90), 100),
    aspect(GeneratorRandom::getInstance().getRandomInt(30, 90), 100),
    vizibilitate(STAT_VIZIBILITATE_INIT, STAT_VIZIBILITATE_MAX),
    bani(0.0, BANI_MAX_INIT)
{}

Statistici::Statistici(const int s, const int f, const int i, const int a, const int v, const double b) :
    sanatate(s, 100),
    fericire(f, 100),
    inteligenta(i, 100),
    aspect(a, 100),
    vizibilitate(v, STAT_VIZIBILITATE_MAX),
    bani(b, BANI_MAX_INIT)
{}

void Statistici::modificaBani(const double suma) {
    this->bani.modifica(suma);

    if (constexpr double PRAG_DATORIE_MAXIMA = -200.0; this->bani.getValoare() < PRAG_DATORIE_MAXIMA) {
        const double ajustare = PRAG_DATORIE_MAXIMA - this->bani.getValoare();
        this->bani.modifica(ajustare);
        std::cout << "\n[! DATORIE MAXIMA ATINSĂ!] Pierdere Fericire.";
        modificaStatistica("Fericire", -2);
    }
}

void Statistici::modificaStatistica(const std::string& tip, const int valoare) {

    if (tip == "Sanatate") {
        sanatate.modifica(valoare);
    }
    else if (tip == "Fericire") {
        fericire.modifica(valoare);
    }
    else if (tip == "Inteligenta") {
        inteligenta.modifica(valoare);
    }
    else if (tip == "Aspect") {
        aspect.modifica(valoare);
    }
    else if (tip == "Vizibilitate") {
        vizibilitate.modifica(valoare);
    }
    else {
        std::cerr << "[EROARE LOGICA] Statistica necunoscuta: " << tip << std::endl;
    }
}

[[nodiscard]] const StatisticaLimita<int>& Statistici::getInteligenta() const { return inteligenta; }
[[nodiscard]] const StatisticaLimita<int>& Statistici::getFericire() const { return fericire; }
[[nodiscard]] const StatisticaLimita<int>& Statistici::getAspect() const { return aspect; }
[[nodiscard]] const StatisticaLimita<int>& Statistici::getSanatate() const { return sanatate; }

double Statistici::getBani() const { return this->bani.getValoare(); }

[[nodiscard]] bool Statistici::areStatisticiSanatoase() const {
    constexpr int PRAG_SANATATE_BINE = 50;
    return sanatate.getValoare() > PRAG_SANATATE_BINE && fericire.getValoare() > PRAG_SANATATE_BINE;
}

double Statistici::getMedieFericireIstoric() const {
    return fericire.getMedieIstoric();
}
double Statistici::getStatistica(const std::string& numeStatistica) const {

    if (numeStatistica == "Sanatate") {
        return sanatate.getValoare();
    }
    else if (numeStatistica == "Fericire") {
        return fericire.getValoare();
    }
    else if (numeStatistica == "Inteligenta") {
        return inteligenta.getValoare();
    }
    else if (numeStatistica == "Aspect") {
        return aspect.getValoare();
    }
    else if (numeStatistica == "Vizibilitate") {
        return vizibilitate.getValoare();
    }
    else if (numeStatistica == "Bani") {
        return bani.getValoare();
    }
    else {
        std::cerr << "[EROARE LOGICA] Tentativa de citire a unei statistici necunoscute: " << numeStatistica << std::endl;
        return 0.0;
    }
}

std::ostream& operator<<(std::ostream& os, const Statistici& s) {
    os << "Sanatate: " << s.sanatate.getValoare()
       << ", Fericire: " << s.fericire.getValoare()
       << ", Inteligenta: " << s.inteligenta.getValoare()
       << ", Aspect: " << s.aspect.getValoare()
       << ", Vizibilitate: " << s.vizibilitate.getValoare()
       << ", BANI: " << std::fixed << std::setprecision(2) << s.bani.getValoare() << "K";
    return os;
}