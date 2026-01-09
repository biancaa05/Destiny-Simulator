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
    StatisticaLimita<int> vizibilitate;
    StatisticaLimita<double> bani;

public:
    Statistici();
    Statistici(int s, int f, int i, int a, int v, double b);
    Statistici(const Statistici& other) = default;
    Statistici& operator=(const Statistici& other) = default;
    Statistici(Statistici&& other) = default;
    Statistici& operator=(Statistici&& other) = default;

    [[nodiscard]] const StatisticaLimita<int> &getInteligenta() const;
    [[nodiscard]] const StatisticaLimita<int> &getFericire() const;
    [[nodiscard]] const StatisticaLimita<int> &getAspect() const;
    [[nodiscard]] const StatisticaLimita<int> &getSanatate() const;
    [[nodiscard]] const StatisticaLimita<int>& getVizibilitate() const { return vizibilitate; }

    [[nodiscard]] bool areStatisticiSanatoase() const;
    [[nodiscard]] double getMedieFericireIstoric() const;
    [[nodiscard]] double getStatistica(const std::string& numeStatistica) const;
    friend std::ostream& operator<<(std::ostream& os, const Statistici& s);

    void modificaBani(double suma);
    [[nodiscard]] double getBani() const;

    void modificaStatistica(const std::string& tip, int valoare);
};

#endif // STATISTICI_H