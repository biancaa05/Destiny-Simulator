#ifndef RETEASOCIALA_H
#define RETEASOCIALA_H

#include "Relatie.h"
#include <string>
#include <vector>

enum TipActiune {
    POSTARE_SIMPLA = 1,
    POSTARE_VIRALA,
    POSTARE_CONTROVERSATA,
    DEZACTIVARE
};

class ReteaSociala {
private:
    std::string platforma = "LifeFeed";

public:

    ReteaSociala() = default;
    void posteazaCeva(Statistici& stats, std::vector<Relatie>& relatii) const;
    friend std::ostream& operator<<(std::ostream& os, const ReteaSociala& r);
};

#endif // RETEASOCIALA_H