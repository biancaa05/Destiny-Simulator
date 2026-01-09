#ifndef GENERATORRANDOM_H
#define GENERATORRANDOM_H

#include <random>

class GeneratorRandom {
private:
    std::random_device rd;
    std::mt19937 generator;

    GeneratorRandom() : generator(rd()) {}
public:
    GeneratorRandom(const GeneratorRandom&) = delete;
    GeneratorRandom& operator=(const GeneratorRandom&) = delete;

    static GeneratorRandom& getInstance() {
        static GeneratorRandom instance;
        return instance;
    }

    int getRandomInt(const int min, const int max) {
        if (min > max) return min;
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(generator);
    }

    double getDouble(const double min, const double max) {
        std::uniform_real_distribution<> distrib(min, max);
        return distrib(generator);
    }

    bool getChance(const double procent) {
        return getDouble(0.0, 1.0) < procent;
    }
};

#endif // GENERATORRANDOM_H