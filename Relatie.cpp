#include "Relatie.h"

Relatie::Relatie(const std::string& nume, const std::string& tip, const int afectiune) {
    this->numePersoana = nume;
    this->tipRelatie = tip;
    this->nivelAfectiune = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, afectiune));
}
Relatie::Relatie(const Relatie& other) {
    this->numePersoana = other.numePersoana;
    this->tipRelatie = other.tipRelatie;
    this->nivelAfectiune = other.nivelAfectiune;
}
Relatie& Relatie::operator=(const Relatie& other) {
    if (this != &other) {
        this->numePersoana = other.numePersoana;
        this->tipRelatie = other.tipRelatie;
        this->nivelAfectiune = other.nivelAfectiune;
    }
    return *this;
}

[[nodiscard]] int Relatie::getNivelAfectiune() const { return nivelAfectiune; }
[[nodiscard]] const std::string& Relatie::getNumePersoana() const { return numePersoana; }
[[nodiscard]] const std::string& Relatie::getTipRelatie() const { return tipRelatie; }

void Relatie::imbunatatesteRelatia(const int puncte) {
    nivelAfectiune = std::min(VAL_MAX_STAT, nivelAfectiune + puncte);
}

std::ostream& operator<<(std::ostream& os, const Relatie& r) {
    os << r.tipRelatie << " (" << r.numePersoana << ") - Afectiune: " << r.nivelAfectiune;
    return os;
}