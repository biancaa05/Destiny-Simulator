#ifndef STATISTICALIMITA_H
#define STATISTICALIMITA_H

#include <vector>
#include <algorithm>
#include <numeric>

template <typename T>
class StatisticaLimita {
private:
    T valoare;
    T valoareMaxima;
    std::vector<T> istoricValori;

public:
    StatisticaLimita(T val_init, T max) : valoareMaxima(max) {
        if (val_init > valoareMaxima && valoareMaxima > T{}) {
            valoare = valoareMaxima;
        } else if (val_init < T{}) {
            valoare = T{};
        } else {
            valoare = val_init;
        }
        istoricValori.push_back(valoare);
    }

    void modifica(T delta) {
        valoare += delta;

        if (valoareMaxima > T{}) {
            valoare = std::min(valoare, valoareMaxima);
            valoare = std::max(valoare, T{});
        }

        istoricValori.push_back(valoare);
    }

    T getValoare() const { return valoare; }

    T getMedieIstoric() const {
        if (istoricValori.empty()) return T{};
        T suma = std::accumulate(istoricValori.begin(), istoricValori.end(), T{});
        return suma / istoricValori.size();
    }
};

#endif // STATISTICALIMITA_H