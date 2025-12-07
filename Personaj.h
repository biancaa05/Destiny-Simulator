#ifndef PERSONAJ_H
#define PERSONAJ_H

#include <memory>
#include "sim_common.h"
#include "Statistici.h"
#include "Cariera.h"
#include "Relatie.h"
#include "Aventura.h"
#include "EvenimentViata.h"

class Personaj {
    std::string numeComplet;
    std::string nationalitate;
    DataNastere dataNastere;
    int varsta;
    int varstaDecesAleatorie;
    bool esteMort;
    int bonusAventuraTemporar = 0;
    Statistici stats;
    Cariera cariera;
    std::vector<Relatie> relatii;
    std::unique_ptr<Aventura> ultimaAventura{};
    std::vector<EvenimentViata> istoricEvenimente;

    static constexpr int MAX_RELATII = 5;
    static int nrPersonajeActive;

    void adaugaRelatieIntern(const Relatie& r);
    void creeazaRelatieRandom();
    void marcheazaDeces(const std::string& cauza);
    bool verificaDeces();
    void evenimentAleatoriu();
    void obtinePrimulJob();
    void intretinereFinanciara();
    void gestioneazaExpeditiePericuloasa();
    void reseteazaBonusAventura();

public:
    static void afiseazaMeniuDecizie();
    static int getNrPersonajeActive();

    Personaj(const std::string& nume, const std::string& nat, int v_init, const DataNastere& dn, Statistici initialStats);
    Personaj(const std::string& nume, const std::string& nat, int v_init, const DataNastere& dn);

    Personaj(const Personaj& other);
    Personaj& operator=(const Personaj& other);
    Personaj(Personaj&& other) = default;
    Personaj& operator=(Personaj&& other) = delete;
    ~Personaj();

    void incepeRelatieNoua(const std::string& nume, const std::string& tip, int afectiune);
    void iaDecizieDestin(int alegere);
    void adaugaEveniment(int v_init, const std::string& descriere, const std::string& impact);
    void afiseazaIstoricViata() const;
    void sePreparaPentruAventura();

    [[nodiscard]] int getVarsta() const;
    [[nodiscard]] bool aplicaAnual(int ani = 1);
    [[nodiscard]] bool getEsteMort() const;

    friend std::ostream& operator<<(std::ostream& os, const Personaj& p);
};

#endif // PERSONAJ_H