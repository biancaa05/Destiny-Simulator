#ifndef SENTINTA_H
#define SENTINTA_H

class Sentinta {
private:
    int durataInchisoareAni;
    double amendaValoare;
    int impactCazierFinal;

public:
    Sentinta(const int ani, const double amenda, const int cazier)
        : durataInchisoareAni(ani), amendaValoare(amenda), impactCazierFinal(cazier) {}

    [[nodiscard]] int getDurataInchisoare() const { return durataInchisoareAni; }
    [[nodiscard]] double getAmendaValoare() const { return amendaValoare; }
    [[nodiscard]] int getImpactCazierFinal() const { return impactCazierFinal; }

    void afiseazaDetalii() const;
};

#endif // SENTINTA_H