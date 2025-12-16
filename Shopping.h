#ifndef SHOPPING_H
#define SHOPPING_H

#include <string>
#include <iostream>
#include <utility>
#include "Statistici.h"

class Shopping {
protected:
    std::string numeProdus;
    double costBaza;

public:
    Shopping()=default;
    Shopping(std::string  nume, const double cost) : numeProdus(std::move(nume)), costBaza(cost) {}
    virtual void aplicaImpact(Statistici& stats) const = 0;
    virtual void afiseazaDetalii(std::ostream& os) const {
        os << "Produs: " << numeProdus << ", Cost: " << costBaza << "K";
    }

    [[nodiscard]] virtual Shopping* clone() const = 0;

    [[nodiscard]] double getCost() const { return costBaza; }
    [[nodiscard]] const std::string& getNumeProdus() const { return numeProdus; }

    virtual ~Shopping() = default;
};

class CumparaCasa final : public Shopping {
public:
    CumparaCasa() : Shopping("Casa Noua", 250.0) {}

    void aplicaImpact(Statistici& stats) const override;
    [[nodiscard]] CumparaCasa* clone() const override { return new CumparaCasa(*this); }
};

class SesiuneSpa final : public Shopping {
public:
    SesiuneSpa() : Shopping("Sesiune Spa Lux", 5.0) {}

    void aplicaImpact(Statistici& stats) const override;
    [[nodiscard]] SesiuneSpa* clone() const override { return new SesiuneSpa(*this); }
};

class CumparaCarti final : public Shopping {
public:
    CumparaCarti() : Shopping("Carti Educative", 0.5) {}

    void aplicaImpact(Statistici& stats) const override;
    [[nodiscard]] CumparaCarti* clone() const override { return new CumparaCarti(*this); }
};

class CumparaMasina final : public Shopping {
public:
    CumparaMasina(): Shopping("Masina", 50) {}
    void aplicaImpact(Statistici& stats) const override;
    [[nodiscard]] CumparaMasina* clone() const override { return new CumparaMasina(*this); }
};
class CumparaMedicamente final : public Shopping {public:
    CumparaMedicamente() : Shopping("Medicamente", 0.1) {}
    void aplicaImpact(Statistici& stats) const override;
    [[nodiscard]] CumparaMedicamente* clone() const override { return new CumparaMedicamente(*this); }
};
#endif // SHOPPING_H