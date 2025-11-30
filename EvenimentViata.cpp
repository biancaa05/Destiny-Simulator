#include "EvenimentViata.h"

EvenimentViata::EvenimentViata(const std::string& desc, const std::string& tip, const int val) {
    this->descriere = desc;
    this->tipImpact = tip;
    this->valoareImpact = val;
}
std::ostream& operator<<(std::ostream& os, const EvenimentViata& e) {
    os << e.descriere << " (Impact: " << e.tipImpact << " de " << e.valoareImpact << ")";
    return os;
}