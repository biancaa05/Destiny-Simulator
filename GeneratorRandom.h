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
};

#endif // GENERATORRANDOM_H