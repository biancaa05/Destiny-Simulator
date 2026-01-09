#ifndef SIMUTILITIES_H
#define SIMUTILITIES_H

#include <stdexcept>
#include <vector>

#include "GeneratorRandom.h"

template <typename T>
const T& alegeElementAleatoriu(const std::vector<T>& container) {
    if (container.empty()) {
        throw std::out_of_range("Containerul este gol in alegeElementAleatoriu.");
    }

    const int index_random = GeneratorRandom::getInstance().getRandomInt(
        0, 
        static_cast<int>(container.size()) - 1
    );

    return container[index_random];
}

#endif // SIMUTILITIES_H