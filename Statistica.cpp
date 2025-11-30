#include "Statistica.h"

Statistica::Statistica() {
    this->nume = "Necunoscuta";
    this->valoare = 0;
}

Statistica::Statistica(const std::string& nume, const int valoare) {
    this->nume = nume;
    this->valoare = valoare;
}

Statistica& Statistica::operator=(const Statistica& other) {
    if (this != &other) {
        this->nume = other.nume;
        this->valoare = other.valoare;
    }
    return *this;
}

[[nodiscard]] int Statistica::getValoare() const { return valoare; }

void Statistica::setValoare(const int nouaValoare) {
    this->valoare = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, nouaValoare));
}

std::ostream& operator<<(std::ostream& os, const Statistica& s) {
    os << "[" << s.nume << ": " << s.valoare << "]";
    return os;
}