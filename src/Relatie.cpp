#include "../headers/Relatie.h"

Relatie::Relatie(const std::string& nume, const std::string& tip, const int afectiune, const int varsta) {
    this->numePersoana = nume;
    this->tipRelatie = tip;
    this->nivelAfectiune = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, afectiune));
    this->varsta = varsta;
}
Relatie::Relatie(const Relatie& other) {
    this->numePersoana = other.numePersoana;
    this->tipRelatie = other.tipRelatie;
    this->nivelAfectiune = other.nivelAfectiune;
    this->varsta= other.varsta;
}
Relatie& Relatie::operator=(const Relatie& other) {
    if (this != &other) {
        this->numePersoana = other.numePersoana;
        this->tipRelatie = other.tipRelatie;
        this->nivelAfectiune = other.nivelAfectiune;
        this->varsta = other.varsta;
    }
    return *this;
}

[[nodiscard]] int Relatie::getNivelAfectiune() const { return nivelAfectiune; }
[[nodiscard]] const std::string& Relatie::getNumePersoana() const { return numePersoana; }
[[nodiscard]] const std::string& Relatie::getTipRelatie() const { return tipRelatie; }
[[nodiscard]] int Relatie::getVarsta() const { return varsta; }

void Relatie::imbunatatesteRelatia(const int puncte) {
    nivelAfectiune = std::min(VAL_MAX_STAT, nivelAfectiune + puncte);
}
void Relatie::setVarsta(int v) {
    this->varsta = v;
}
std::ostream& operator<<(std::ostream& os, const Relatie& r) {
    os << r.tipRelatie << " (" << r.numePersoana << ") - Afectiune: " << r.nivelAfectiune;
    return os;
}