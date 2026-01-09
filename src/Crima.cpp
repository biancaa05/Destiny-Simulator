#include "../headers/Crima.h"
#include "../headers/CrimaSimpla.h"
#include "../headers/CrimaViolenta.h"
#include <iostream>

static const CrimaSimpla FURT_BUZUNARE("Furt din Buzunare", 0.20, 1000.0, 5000.0, 5);
static const CrimaSimpla VANDALISM_PUBLIC("Vandalism Public", 0.15, 50.0, 500.0, 2);
static const CrimaSimpla CONTRABANDA_MINORA("Contrabanda Minora", 0.30, 2000.0, 10000.0, 8);
static const CrimaViolenta VANZARE_ILEGALA("Vanzare Ilegala", 0.55, 10000.0, 50000.0, 15, -5.0);
static const CrimaViolenta JAF_ARMAT("Jaf Armat la Banca", 0.80, 500000.0, 2000000.0, 40, -20.0);
static const CrimaViolenta EXTORCARE_FONDURI("Extorcare de Fonduri", 0.65, 50000.0, 150000.0, 25, -10.0);

const Crima* Crima::getCrimaPeDecizie(const int id_decizie) {

    switch (id_decizie) {
        case 20:
            return &FURT_BUZUNARE;
        case 21:
            return &VANDALISM_PUBLIC;
        case 22:
            return &CONTRABANDA_MINORA;

        case 23:
            return &VANZARE_ILEGALA;
        case 24:
            return &JAF_ARMAT;
        case 25:
            return &EXTORCARE_FONDURI;

        default:
            std::cerr << "[EROARE] ID de crima necunoscut: " << id_decizie << std::endl;
            return nullptr;
    }
}