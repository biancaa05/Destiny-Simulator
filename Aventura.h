#ifndef AVENTURA_H
#define AVENTURA_H
#include <utility>
#include "sim_common.h"

class Aventura {
    std::string numeAventura;
    int sansaEsuecBaza;
public:
    Aventura(std::string  nume, const int sansaBaza) :
        numeAventura(std::move(nume)), sansaEsuecBaza(sansaBaza) {}

    [[nodiscard]] virtual Aventura* clone() const = 0;
    virtual void aplicaImpact(Statistici& stats) = 0;

    void afiseazaDetalii(std::ostream& os) const {
        os << "[" << numeAventura << "] Risc Baza: " << sansaEsuecBaza << "%. Rezultate: ";
        afiseaza(os);
    }

    Aventura(const Aventura& other) = default;
    Aventura& operator=(const Aventura& other) = default;
    virtual ~Aventura() = default;

protected:
    virtual void afiseaza(std::ostream& os) const = 0;
};


class DrumetieMontana final : public Aventura {
public:
    DrumetieMontana() : Aventura("Drumetie Montana", 30) {}
    DrumetieMontana(const DrumetieMontana& other) = default;

    [[nodiscard]] Aventura* clone() const override { return new DrumetieMontana(*this); }
    void aplicaImpact(Statistici& stats) override;
private:
    void afiseaza(std::ostream& os) const override;
};


class Salvare final : public Aventura {
public:
    Salvare() : Aventura("Asistenta Salvare", 40) {}
    Salvare(const Salvare& other) = default;

    [[nodiscard]] Aventura* clone() const override { return new Salvare(*this); }
    void aplicaImpact(Statistici& stats) override;
    static void oferaMotivatieExtra(Statistici& stats);
private:
    void afiseaza(std::ostream& os) const override;
};

class Vanatoare final : public Aventura {
public:
    Vanatoare() : Aventura("Vanatoare Animal Salbatic", 70) {}
    Vanatoare(const Vanatoare& other) = default;

    [[nodiscard]] Aventura* clone() const override { return new Vanatoare(*this); }
    void aplicaImpact(Statistici& stats) override;
private:
    void afiseaza(std::ostream& os) const override;
};

#endif // AVENTURA_H