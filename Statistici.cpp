#include "Statistici.h"
#include "Exceptii.h"

void Statistici::randomizeStats() {
    this->sanatate = Statistica("Sanatate", getRandomInt(30, 90));
    this->fericire = Statistica("Fericire", getRandomInt(30, 90));
    this->inteligenta = Statistica("Inteligenta", getRandomInt(30, 90));
    this->aspect = Statistica("Aspect", getRandomInt(30, 90));
    this->bani = 0.0;
}

Statistici::Statistici() : bani(0.0) {randomizeStats();}

Statistici::Statistici(const int s,const int f,const int i,const int a) :
    sanatate("Sanatate", s),
    fericire("Fericire", f),
    inteligenta("Inteligenta", i),
    aspect("Aspect", a),
    bani(0.0)
{}

void Statistici::modificaBani(const double suma) { this->bani += suma; }
double Statistici::getBani() const { return this->bani; }

[[nodiscard]] const Statistica& Statistici::getInteligenta() const { return inteligenta; }
[[nodiscard]] const Statistica& Statistici::getFericire() const { return fericire; }
[[nodiscard]] const Statistica& Statistici::getAspect() const { return aspect; }
[[nodiscard]] const Statistica& Statistici::getSanatate() const { return sanatate; }

void Statistici::modificaStatistica(const std::string& tip, const int valoare) {
    if (tip == "Sanatate") {
        int nouaValoare = sanatate.getValoare() + valoare;
        if (nouaValoare < 0) { nouaValoare = 0; }
        sanatate.setValoare(nouaValoare);
    }
    else if (tip == "Fericire") {
        int nouaValoare = fericire.getValoare() + valoare;
        if (nouaValoare < 0) { nouaValoare = 0; }
        fericire.setValoare(nouaValoare);
    }
    else if (tip == "Inteligenta") {
        inteligenta.setValoare(inteligenta.getValoare() + valoare);
    }
    else if (tip == "Aspect") {
        aspect.setValoare(aspect.getValoare() + valoare);
    }
}

[[nodiscard]] bool Statistici::areStatisticiSanatoase() const {
    constexpr int PRAG_SANATATE_BINE = 50;
    return sanatate.getValoare() > PRAG_SANATATE_BINE && fericire.getValoare() > PRAG_SANATATE_BINE;
}

std::ostream& operator<<(std::ostream& os, const Statistici& s) {
    os << s.sanatate << " " << s.fericire << " " << s.inteligenta << " " << s.aspect;
    return os;
}