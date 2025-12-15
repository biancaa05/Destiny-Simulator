#ifndef STATISTICI_H
#define STATISTICI_H

#include "StatisticaLimita.h"
#include <string>

class Statistici {
private:
    StatisticaLimita<int> sanatate;
    StatisticaLimita<int> fericire;
    StatisticaLimita<int> inteligenta;
    StatisticaLimita<int> aspect;
    StatisticaLimita<double> bani;

public:
    Statistici();
    Statistici(int s, int f, int i, int a);

    Statistici(const Statistici& other) = default;
    Statistici& operator=(const Statistici& other) = default;
    Statistici(Statistici&& other) = default;
    Statistici& operator=(Statistici&& other) = default;

    void modificaStatistica(const std::string& tip, int valoare);

    [[nodiscard]] const StatisticaLimita<int> &getInteligenta() const;
    [[nodiscard]] const StatisticaLimita<int> &getFericire() const;
    [[nodiscard]] const StatisticaLimita<int> &getAspect() const;
    [[nodiscard]] const StatisticaLimita<int> &getSanatate() const;

    [[nodiscard]] bool areStatisticiSanatoase() const;
    friend std::ostream& operator<<(std::ostream& os, const Statistici& s);

    void modificaBani(double suma);
    [[nodiscard]] double getBani() const;
};

#endif // STATISTICI_H