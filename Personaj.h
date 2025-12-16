#ifndef PERSONAJ_H
#define PERSONAJ_H

#include <memory>
#include "sim_common.h"
#include "Statistici.h"
#include "Cariera.h"
#include "Relatie.h"
#include "Aventura.h"
#include "EvenimentViata.h"
#include "ReteaSociala.h"
#include "Shopping.h"

class Sentinta;

class Personaj {
    std::string numeComplet;
    std::string nationalitate;
    DataNastere dataNastere;
    int varsta;
    int varstaDecesAleatorie;
    bool esteMort;
    Statistici stats;
    Cariera cariera;
    std::vector<Relatie> relatii;
    std::unique_ptr<Aventura> ultimaAventura{};
    std::vector<EvenimentViata> istoricEvenimente;
    ReteaSociala retea;
    int aniSentintaRamasi;
    int cazierNivel;
    int bonusAventuraTemporar=0;
    std::string numeSotSauSotie;

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
    void evenimentViataMajor();
    void initializeazaRelatiiParente();
    void cumparaProdus(const Shopping& produs);
    void executaActiuneCrima(int crimaID);

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

    void iaDecizieDestin(int alegere);
    void adaugaEveniment(int v_init, const std::string& descriere, const std::string& impact);
    void afiseazaIstoricViata() const;
    void sePreparaPentruAventura();
    void planificaVacanta();
    void afiseazaVerdictFinal() const;
    void actiuneSocialMedia();
    void aplicaSentinta(const Sentinta& s);
    void ruleazaAnInInchisoare();
    void modificaStatistica(const std::string& nume, double valoare);

    [[nodiscard]] int getVarsta() const;
    [[nodiscard]] bool aplicaAnual(int ani = 1);
    [[nodiscard]] bool getEsteMort() const;
    [[nodiscard]] bool esteInInchisoare() const;
    [[nodiscard]] const Statistici &getStatistici() const;


    friend std::ostream& operator<<(std::ostream& os, const Personaj& p);
};

#endif // PERSONAJ_H