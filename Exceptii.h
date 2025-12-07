#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <string>

class EroareSimulare : public std::exception {
    std::string mesaj;
public:
    explicit EroareSimulare(const std::string& msg) : mesaj("[EROARE SIMULARE] " + msg) {}

    [[nodiscard]] const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class EroareFisier final : public EroareSimulare {
public:
    explicit EroareFisier(const std::string& nume_fisier) :
        EroareSimulare("I/O: Nu s-a putut deschide sau citi din fisierul: " + nume_fisier) {}
};

class EroareDecizieInvalida final : public EroareSimulare {
public:
    explicit EroareDecizieInvalida(const int decizie) :
        EroareSimulare("Validare Logica: Optiunea " + std::to_string(decizie) +
                       " este invalida. Deciziile valide sunt [0, 4].") {}
};

class EroareSimulareDurata : public EroareSimulare {
public:
    EroareSimulareDurata() :
        EroareSimulare("Structura Date: Numarul de ani de simulat este zero/negativ sau vectorul de decizii este incomplet.") {}
};


#endif // EXCEPTII_H