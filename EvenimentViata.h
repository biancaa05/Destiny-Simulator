#ifndef EVENIMENTVIATA_H
#define EVENIMENTVIATA_H

#include <iostream>
#include <string>

class EvenimentViata {
    int varsta{};
    std::string descriere;
    std::string impactDetalii;

public:
    EvenimentViata()=default;
    EvenimentViata(const int varsta, std::string descriere, std::string impact) :
        varsta(varsta), descriere(std::move(descriere)), impactDetalii(std::move(impact)) {}

    EvenimentViata(const EvenimentViata& other) = default;
    EvenimentViata& operator=(const EvenimentViata& other) = default;
    ~EvenimentViata()=default;

    friend std::ostream& operator<<(std::ostream& os, const EvenimentViata& ev);
};

#endif // EVENIMENTVIATA_H