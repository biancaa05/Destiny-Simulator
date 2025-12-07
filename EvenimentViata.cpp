#include "EvenimentViata.h"
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const EvenimentViata& ev) {
    os << "[" << ev.varsta << " ani] ";
    os << ev.descriere;
    os << " (Impact: " << ev.impactDetalii << ")";
    return os;
}