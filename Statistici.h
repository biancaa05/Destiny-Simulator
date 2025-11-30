#ifndef STATISTICI_H
#define STATISTICI_H

#include "Statistica.h"

class Statistici {
    Statistica sanatate;
    Statistica fericire;
    Statistica inteligenta;
    Statistica aspect;

    void randomizeStats();

public:
    Statistici();
    Statistici(int s, int f, int i, int a);

    Statistici(const Statistici& other) = default;
    Statistici& operator=(const Statistici& other) = default;
    Statistici(Statistici&& other) = default;
    Statistici& operator=(Statistici&& other) = default;

    [[nodiscard]] const Statistica& getInteligenta() const;
    [[nodiscard]] const Statistica& getFericire() const;
    [[nodiscard]] const Statistica& getAspect() const;
    [[nodiscard]] const Statistica& getSanatate() const;

    void modificaStatistica(const std::string& tip, int valoare);

    [[nodiscard]] bool areStatisticiSanatoase() const;
    friend std::ostream& operator<<(std::ostream& os, const Statistici& s);
};

#endif // STATISTICI_H