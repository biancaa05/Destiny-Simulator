#ifndef STATISTICA_H
#define STATISTICA_H

#include "sim_common.h"

class Statistica {
    int valoare;
    std::string nume;
public:
    Statistica();
    Statistica(const std::string& nume, int valoare);
    Statistica(const Statistica& other) = default;
    Statistica& operator=(const Statistica& other);
    Statistica(Statistica&& other) = default;
    Statistica& operator=(Statistica&& other) = default;

    [[nodiscard]] int getValoare() const;

    friend std::ostream& operator<<(std::ostream& os, const Statistica& s);
};

#endif // STATISTICA_H