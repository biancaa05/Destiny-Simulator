#ifndef AVENTURA_H
#define AVENTURA_H
#include <utility>
#include "sim_common.h"

class Aventura {
protected:
    std::string numeAventura;
    int sansaEsecBaza;

    virtual void afiseaza(std::ostream& os) const = 0;

public:
    Aventura(std::string  nume, const int sansaBaza) :
        numeAventura(std::move(nume)), sansaEsecBaza(sansaBaza) {}

    [[nodiscard]] virtual Aventura* clone() const = 0;
    virtual void aplicaImpact(Statistici& stats) = 0;

    [[nodiscard]] int getSansaEsecBaza() const;
    void afiseazaDetalii(std::ostream& os) const {
        os << "[" << numeAventura << "] Risc Baza: " << sansaEsecBaza << "%. Rezultate: ";
        afiseaza(os);
    }

    Aventura(const Aventura& other) = default;
    Aventura& operator=(const Aventura& other) = default;
    virtual ~Aventura() = default;

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

class NouHobby final : public Aventura {
public:
    NouHobby() : Aventura("Incercare Nou Hobby", 40) {}
    NouHobby(const NouHobby& other) = default;

    [[nodiscard]] Aventura* clone() const override { return new NouHobby(*this); }
    void aplicaImpact(Statistici& stats) override;
private:
    void afiseaza(std::ostream& os) const override;
};

class EvenimentSocial final : public Aventura {
public:
    EvenimentSocial() : Aventura("Eveniment Social Important", 25) {}
    EvenimentSocial(const EvenimentSocial& other) = default;

    [[nodiscard]] Aventura* clone() const override { return new EvenimentSocial(*this); }
    void aplicaImpact(Statistici& stats) override;
private:
    void afiseaza(std::ostream& os) const override;
};

class ScandalPublic final : public Aventura {
public:
    ScandalPublic() : Aventura("Scandal Public/Criză de Reputație", 60) {}
    ScandalPublic(const ScandalPublic& other) = default;

    [[nodiscard]] Aventura* clone() const override { return new ScandalPublic(*this); }
    void aplicaImpact(Statistici& stats) override;
private:
    void afiseaza(std::ostream& os) const override;
};
#endif // AVENTURA_H