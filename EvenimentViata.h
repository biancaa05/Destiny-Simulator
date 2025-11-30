#ifndef EVENIMENTVIATA_H
#define EVENIMENTVIATA_H

#include "sim_common.h"

class EvenimentViata {
private:
    std::string descriere;
    std::string tipImpact;
    int valoareImpact;
public:
    EvenimentViata(const std::string& desc, const std::string& tip, int val);
    friend std::ostream& operator<<(std::ostream& os, const EvenimentViata& e);
};

#endif