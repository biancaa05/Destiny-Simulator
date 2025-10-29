#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <random>

// Nu mai avem nevoie de <stdlib.h> sau <ctime>

// Constante globale
const int VAL_MIN_STAT = 0;
const int VAL_MAX_STAT = 100;
const int VITEZA_DEGRADARE_FERICIRE = 1;
const int PRAG_SANATATE_CRITICA = 5;
const int MAX_OPTIUNI_DECIZIE = 3;
const int VARSTA_MAXIMA_FORTATA = 100;

// Vector de nume și statusuri pentru randomizare
const std::vector<std::string> NUME_RELATII = {"Alexandru", "Diana", "Mihai", "Elena", "Andrei", "Ioana"};
const std::vector<std::string> STATUS_RELATII = {"Partener", "Prieten", "Coleg", "Ruda"};

int getRandomInt(int min, int max) {
    static std::random_device rd;      // seed hardware
    static std::mt19937 gen(rd());     // generator Mersenne Twister
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Functie helper pentru citirea si validarea deciziei
int citesteAlegere() {
    int alegere = 0;
    std::cout << "\n> ACTIUNE JUCATOR: Introduceti numarul deciziei (1-" << MAX_OPTIUNI_DECIZIE << ", sau 0 pentru a inainta timpul): ";

    while (!(std::cin >> alegere) || alegere < 0 || alegere > MAX_OPTIUNI_DECIZIE) {
        std::cout << "Optiune invalida. Va rugam introduceti un numar valid (0-" << MAX_OPTIUNI_DECIZIE << "): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return alegere;
}

// ====================================================================
// Structură și Funcții pentru Data de Naștere
// ====================================================================

struct DataNastere {
    int zi=0;
    int luna=0;
    const int an = 2025;
};

// Functie helper care mapeaza luna din int in string
std::string getLunaString(int luna) {
    static const std::vector<std::string> numeLuni = {
        "", "Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie",
        "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"
    };
    return numeLuni[luna];
}

// Functie helper pentru generarea datelor
DataNastere genereazaDataNastere() {
    DataNastere dn;
    dn.luna = getRandomInt(1, 12);

    int zile_max;
    if (dn.luna == 2) {
        zile_max = 28;
    } else if (dn.luna == 4 || dn.luna == 6 || dn.luna == 9 || dn.luna == 11) {
        zile_max = 30;
    } else {
        zile_max = 31;
    }

    dn.zi = getRandomInt(1, zile_max);
    return dn;
}

// Operator<< pentru afisarea datei (ex: 28 Martie 2025)
std::ostream& operator<<(std::ostream& os, const DataNastere& dn) {
    os << dn.zi << " " << getLunaString(dn.luna) << " " << dn.an;
    return os;
}


// ====================================================================
// CLASA 1: STATISTICA
// ====================================================================
class Statistica {
private:
    int valoare; std::string nume;
public:
    Statistica() {this->nume = "Necunoscuta"; this->valoare = 0;}
    Statistica(const std::string& nume, int valoare) {this->nume = nume; this->valoare = valoare;}
    Statistica& operator=(const Statistica& other) {
        if (this != &other) {this->nume = other.nume; this->valoare = other.valoare;} return *this;}
    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] int getValoare() const { return valoare; }
    void setValoare(int nouaValoare) {
        this->valoare = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, nouaValoare));
    }
    [[nodiscard]] bool esteCritica() const { return valoare < PRAG_SANATATE_CRITICA; }
    friend std::ostream& operator<<(std::ostream& os, const Statistica& s);
};
std::ostream& operator<<(std::ostream& os, const Statistica& s) {os << "[" << s.nume << ": " << s.valoare << "]"; return os;}


// ====================================================================
// CLASA 2: STATISTICI
// ====================================================================
class Statistici {
private: Statistica sanatate; Statistica fericire; Statistica inteligenta; Statistica aspect;
    void randomizeStats() {
        this->sanatate = Statistica("Sanatate", getRandomInt(30, 90)); this->fericire = Statistica("Fericire", getRandomInt(30, 90));
        this->inteligenta = Statistica("Inteligenta", getRandomInt(30, 90)); this->aspect = Statistica("Aspect", getRandomInt(30, 90));}
public:
    [[nodiscard]] const Statistica& getInteligenta() const { return inteligenta; }
    [[nodiscard]] const Statistica& getFericire() const { return fericire; }
    [[nodiscard]] const Statistica& getAspect() const { return aspect; }
    [[nodiscard]] const Statistica& getSanatate() const { return sanatate; }
    Statistici() {randomizeStats();}
    void modificaStatistica(const std::string& tip, int valoare) {
        if (tip == "Sanatate") {sanatate.setValoare(sanatate.getValoare() + valoare);} else if (tip == "Fericire") {fericire.setValoare(fericire.getValoare() + valoare);}
        else if (tip == "Inteligenta") {inteligenta.setValoare(inteligenta.getValoare() + valoare);} else if (tip == "Aspect") {aspect.setValoare(aspect.getValoare() + valoare);}}
    [[nodiscard]] bool areStatisticiSanatoase() const {const int PRAG_SANATATE_BINE = 50; return sanatate.getValoare() > PRAG_SANATATE_BINE && fericire.getValoare() > PRAG_SANATATE_BINE;}
    friend std::ostream& operator<<(std::ostream& os, const Statistici& s);
};
std::ostream& operator<<(std::ostream& os, const Statistici& s) {os << s.sanatate << " " << s.fericire << " " << s.inteligenta << " " << s.aspect; return os;}


// ====================================================================
// CLASA 3: CARIERA
// ====================================================================
class Cariera {
private:
    std::string numeJob;
    int salariuAnual;
    int satisfactie;
    int cerintaInteligenta;
    void ajusteazaSatisfactia(int val) {
        satisfactie = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, val));
    }
public:
    Cariera() {
        this->numeJob = "Somaj";
        this->salariuAnual = 0;
        this->satisfactie = 10;
        this->cerintaInteligenta = 0;
    }
    Cariera(const std::string& nume, int salariu, int satisf, int cerinta) {
        this->numeJob = nume;
        this->salariuAnual = salariu;
        this->satisfactie = satisf;
        this->cerintaInteligenta = cerinta;
    }
    [[nodiscard]] const std::string& getNumeJob() const { return numeJob; }
    [[nodiscard]] int getSalariuAnual() const { return salariuAnual; }
    [[nodiscard]] int getCerintaInteligenta() const { return cerintaInteligenta; }
    int incearcaPromovare(int inteligentaPersonaj) {
        int sansaBaza = (inteligentaPersonaj - cerintaInteligenta) / 2;
        int sansaFinala = std::max(10, std::min(90, sansaBaza + getRandomInt(-10, 10)));
        std::cout << "* PROMOVARE: Sansa de succes: " << sansaFinala << "%. ";
        if (getRandomInt(1, 100) <= sansaFinala) {salariuAnual += 20; satisfactie += 15; ajusteazaSatisfactia(satisfactie); std::cout << "SUCCES! Salariu: +" << 20 << ", Satisfactie: +15." << std::endl; return 1;}
        else {std::cout << "ESEC. Nu esti inca pregatit." << std::endl; return 0;}
    }
    friend std::ostream& operator<<(std::ostream& os, const Cariera& c);
};
std::ostream& operator<<(std::ostream& os, const Cariera& c) {os << "Job: " << c.numeJob << " | Salariu: " << c.salariuAnual << "K | Satisfactie: " << c.satisfactie << "%"; return os;}


// ====================================================================
// CLASA 4: RELATIE (Simplificată)
// ====================================================================
class Relatie {
private: std::string numePersoana; std::string tipRelatie; int nivelAfectiune;
public:
    Relatie(const std::string& nume, const std::string& tip, int afectiune) {
        this->numePersoana = nume;
        this->tipRelatie = tip;
        this->nivelAfectiune = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, afectiune));}
    Relatie(const Relatie& other) = default;
    ~Relatie() = default;
    [[nodiscard]] const std::string& getNumePersoana() const { return numePersoana; }
    void imbunatatesteRelatia(int puncte) {
        nivelAfectiune = std::min(VAL_MAX_STAT, nivelAfectiune + puncte); }
    friend std::ostream& operator<<(std::ostream& os, const Relatie& r);
};
std::ostream& operator<<(std::ostream& os, const Relatie& r) {
    os << r.tipRelatie << " (" << r.numePersoana << ") - Afecțiune: " << r.nivelAfectiune; return os;}


// ====================================================================
// CLASA 5: EVENIMENTVIATA
// ====================================================================
class EvenimentViata {
private: std::string descriere; std::string tipImpact; int valoareImpact;
public:
    EvenimentViata(const std::string& desc, const std::string& tip, int val) {
        this->descriere = desc; this->tipImpact = tip; this->valoareImpact = val;}
    [[nodiscard]] const std::string& getTipImpact() const { return tipImpact; }
    [[nodiscard]] int getValoareImpact() const { return valoareImpact; }
    friend std::ostream& operator<<(std::ostream& os, const EvenimentViata& e);
};
std::ostream& operator<<(std::ostream& os, const EvenimentViata& e) {
    os << e.descriere << " (Impact: " << e.tipImpact << " de " << e.valoareImpact << ")"; return os;}


// ====================================================================
// CLASA 6: PERSONAJ
// ====================================================================
class Personaj {
private:
    std::string numeComplet;
    std::string nationalitate;
    DataNastere dataNastere;
    int varsta;
    double bani;
    int varstaDecesAleatorie;
    bool esteMort;
    Statistici stats;
    Cariera cariera;
    std::vector<Relatie> relatii;
    static const int MAX_RELATII = 5;

    void adaugaRelatieIntern(const Relatie& r) {
        if (relatii.size() < MAX_RELATII) {relatii.push_back(r);} else {std::cout << "[LOG] Limita de relatii atinsa." << std::endl;} }

    void creeazaRelatieRandom() {
        if (relatii.size() < MAX_RELATII) {
            // CORECTAT: Cast explicit la int pentru a evita Narrowing Conversion
            const std::string& nume = NUME_RELATII[getRandomInt(0, static_cast<int>(NUME_RELATII.size()) - 1)];
            const std::string& status = STATUS_RELATII[getRandomInt(0, static_cast<int>(STATUS_RELATII.size()) - 1)];
            int afectiune = getRandomInt(20, 95);
            Relatie r(nume, status, afectiune);
            adaugaRelatieIntern(r);
            std::cout << "Noua relatie: " << status << " cu " << nume << " (Afecțiune: " << afectiune << ")." << std::endl;
        }
    }

    void marcheazaDeces(const std::string& cauza) {
        if (esteMort) return; esteMort = true;
        std::cout << "\n=================================================" << std::endl;
        std::cout << "=== JOC TERMINAT! Jucatorul a murit la " << varsta << " ani. ===" << std::endl;
        std::cout << "=== Cauza: " << cauza << " ===" << std::endl;
        std::cout << "=================================================" << std::endl;}

    bool verificaDeces() {
        if (esteMort) return true;
        if (stats.getSanatate().getValoare() < PRAG_SANATATE_CRITICA) {
            marcheazaDeces("Sanatate extrem de scazuta"); return true; }
        return false;
    }

    void evenimentAleatoriu() {
        int sansa = getRandomInt(1, 100);
        if (sansa < 35) {
            std::string tipImpact = "Fericire"; int impact = getRandomInt(-15, 20);
            std::string descriere = "S-a intamplat un eveniment minor in viata ta.";

            if (impact < 0) {descriere = "Ai pierdut un obiect valoros."; stats.modificaStatistica("Fericire", impact);}
            else if (impact > 10) {descriere = "Ai gasit 50 de lei pe strada!"; bani += 0.05; stats.modificaStatistica("Fericire", impact);}
            else {stats.modificaStatistica("Fericire", impact);}

            std::cout << "* Eveniment Aleatoriu: " << descriere << " (" << tipImpact << " " << (impact > 0 ? "+" : "") << impact << ")" << std::endl;

            if (getRandomInt(1, 10) > 8) {
                creeazaRelatieRandom();
            }
        }
    }

    void intretinereFinanciara() {
        const double COST_VIATA = 10.0;
        bani += cariera.getSalariuAnual(); bani -= COST_VIATA;
        if (bani < 0) {std::cout << "[LOG] Ai intrat in datorii! Fericirea scade. "; stats.modificaStatistica("Fericire", -10);}
    }

public:
    void afiseazaMeniuDecizie() {
        std::cout << "\n--- OPTIUNI ANUALE ---" << std::endl;
        std::cout << "1. Incearca Promovarea/Studiu (Inteligenta)" << std::endl;
        std::cout << "2. Expediție Periculoasă (Sanatate/Risc)" << std::endl;
        std::cout << "3. Creeaza o noua relatie (Afectiune si Status Random)" << std::endl;
        std::cout << "0. Continua la Anul Urmator (Fara Decizie majora)" << std::endl;
    }

    Personaj(const std::string& nume, const std::string& nat, int varsta, const DataNastere& dn)
        : dataNastere(dn)
    {
        this->numeComplet = nume;
        this->nationalitate = nat;
        this->varsta = varsta;
        this->bani = 50.0;
        this->varstaDecesAleatorie = getRandomInt(70, 99);
        this->esteMort = false;
        std::cout << "[LOG] Vârsta de deces natural setată la " << this->varstaDecesAleatorie << " ani." << std::endl;
    }
    [[nodiscard]] const std::string& getNume() const { return numeComplet; }
    [[nodiscard]] int getVarsta() const { return varsta; }
    // Funcție complexă 1: Funcția Anuală (Verifică Decesul)
    [[nodiscard]] bool aplicaAnual(int ani = 1) {
        if (esteMort) return true;

        for (int i = 0; i < ani; ++i) {
            // Condiție de deces forțată la 100 de ani
            if (varsta >= varstaDecesAleatorie || varsta >= VARSTA_MAXIMA_FORTATA) {
                marcheazaDeces("Batranete");
                return true;
            }

            varsta += 1;
            std::cout << "\n=================================================" << std::endl;
            std::cout << "--- ANUL " << varsta << " | " << numeComplet << " ---" << std::endl;
            std::cout << "=================================================" << std::endl;
            std::cout << *this << std::endl; // Afișează starea la începutul anului

            // 1. Decizie Jucător
            afiseazaMeniuDecizie();
            int alegere = citesteAlegere();

            if (alegere != 0) {
                iaDecizieDestin(alegere);
            }
            if (verificaDeces()) return true;

            // 2. Progres Anual Automat
            stats.modificaStatistica("Fericire", -VITEZA_DEGRADARE_FERICIRE);

            if (verificaDeces()) return true;
            intretinereFinanciara();
            evenimentAleatoriu();
            if (verificaDeces()) return true;

            for (Relatie& r : relatii) {r.imbunatatesteRelatia(-2);}
        }
        return esteMort;
    }

    void schimbaCariera(const Cariera& c) {
        if (esteMort) return;
        if (stats.getInteligenta().getValoare() >= c.getCerintaInteligenta()) {
            this->cariera = c; std::cout << "[LOG] Te-ai angajat ca " << c.getNumeJob() << "!" << std::endl;
        } else {std::cout << "[LOG] Esec! Nu esti suficient de inteligent (necesar: " << c.getCerintaInteligenta() << ")." << std::endl;}
    }

    void aplicaEveniment(const EvenimentViata& ev) {
        if (esteMort) return;
        std::cout << "\n* EVENIMENT MAJOR: " << ev << std::endl;
        stats.modificaStatistica(ev.getTipImpact(), ev.getValoareImpact());
        verificaDeces();
    }

    void incepeRelatieNoua(const std::string& nume, const std::string& tip, int afectiune) {
        if (esteMort) return; Relatie r(nume, tip, afectiune); adaugaRelatieIntern(r);
    }

    void iaDecizieDestin(int alegere) {
        if (esteMort) return;
        std::cout << "--- EXECUT DECZIA " << alegere << " ---" << std::endl;
        int noroc = getRandomInt(1, 10);
        int impactFericire = 0; int impactAspect = 0; int impactSanatate = 0; int impactInteligenta = 0;

        switch (alegere) {
            case 1: { // Promovare/Studiu
                std::cout << "Actiune: Incearca Promovarea. ";
                if (cariera.getNumeJob() != "Somaj") {cariera.incearcaPromovare(stats.getInteligenta().getValoare()); impactFericire += 10;}
                else {std::cout << "Nu ai job. Ai studiat degeaba. "; impactInteligenta += 5;} break; }
            case 2: { // Expediție Periculoasă
                std::cout << "Actiune: Expediție periculoasă. ";
                if (stats.areStatisticiSanatoase() && noroc > 5) {
                    impactInteligenta = 10;
                    impactFericire = 15;
                    impactAspect = 0;  // supraviețuire fără efect asupra aspectului
                    std::cout << "Ai supraviețuit. Inteligenta +10, Fericire +15.";
                } else {
                    impactSanatate = -40;
                    impactAspect = -20; // a pierdut aspect din cauza rănilor
                    std::cout << "Te-ai îmbolnăvit/Rănit. Sanatate -40, Aspect -20.";
                }
                break;
            }
            case 3: { // Creeaza o noua relatie random
                std::cout << "Actiune: Intalnesti pe cineva nou. ";
                creeazaRelatieRandom();
                impactFericire += 15;
                break;}
            default: {std::cout << "Alegere invalidă. Nicio acțiune." << std::endl;}
        }

        if (impactFericire != 0) stats.modificaStatistica("Fericire", impactFericire);
        if (impactAspect != 0) stats.modificaStatistica("Aspect", impactAspect);
        if (impactSanatate != 0) stats.modificaStatistica("Sanatate", impactSanatate);
        if (impactInteligenta != 0) stats.modificaStatistica("Inteligenta", impactInteligenta);

        verificaDeces();
    }

    [[nodiscard]] bool getEsteMort() const {return esteMort;}

    friend std::ostream& operator<<(std::ostream& os, const Personaj& p);
};

std::ostream& operator<<(std::ostream& os, const Personaj& p) {
    os << "--- " << p.numeComplet << " (" << p.varsta << " ani, " << p.nationalitate << ") ---" << "\n";
    os << " Data Nastere: " << p.dataNastere << "\n"; // Afisare data de nastere
    os << " BANI: " << p.bani << "K | ";
    os << p.cariera << "\n";
    os << " STATISTICI: " << p.stats << "\n";
    os << " RELATII: " << "\n";
    if (p.relatii.empty()) {
         os << "  (Nicio relatie activa)" << "\n";
    } else {
        for (const auto& r : p.relatii) {
            os << "  - " << r << "\n";
        }
    }
    return os;
}

// Functie pentru citirea datelor de la tastatura/fisier
void citesteDateIntrare(std::string& nume, std::string& prenume, std::string& nationalitate) {
    std::ifstream fisierTastatura("tastatura.txt");
    if (fisierTastatura.is_open()) {

        if (fisierTastatura >> nume >> prenume >> nationalitate) {
            // Success
        } else {
             std::cout << "Eroare citire date din tastatura.txt. Verificati formatul (Nume Prenume Nationalitate)! Folosind date implicite." << std::endl;
             nume = "Andrei";
             prenume = "Popescu";
             nationalitate = "Roman";
        }
        fisierTastatura.close();
    } else {
        std::cout << "Eroare la deschiderea fisierului tastatura.txt! Asigurati-va ca exista. Folosind date implicite." << std::endl;
        nume = "Andrei";
        prenume = "Popescu";
        nationalitate = "Roman";
    }
}

// ====================================================================
// FUNCTIA MAIN
// ====================================================================

int main() {
    std::cout << "--- BITLIFE: DESTINY SIMULATOR v3.1 (FINAL CLEAN) ---" << std::endl;

    // 1. Citirea datelor inițiale din tastatura.txt
    std::string nume, prenume, nationalitate;
    citesteDateIntrare(nume, prenume, nationalitate);

    std::string numeComplet = nume + " " + prenume;
    DataNastere dn = genereazaDataNastere();

    std::cout << "\n=================================================" << std::endl;
    std::cout << "Numele personajului: " << nume << std::endl;
    std::cout << "Prenumele personajului: " << prenume << std::endl;
    std::cout << "Nationalitatea personajului: " << nationalitate << std::endl;
    std::cout << "Data de nastere (Anul 2025): " << dn << std::endl;
    std::cout << "=================================================" << std::endl;

    // 2. Crearea de obiecte
    Personaj jucator(numeComplet, nationalitate, 1, dn); // începe de la 1 an

    Cariera jobSimplu("Casier", 30, 40, 30);
    Cariera jobAvansat("Programator Senior", 120, 80, 85);
    EvenimentViata imbolnavire("Gripa severa", "Sanatate", -30);

    // Relații de familie la naștere
    jucator.incepeRelatieNoua("Maria", "Mama", 100);
    jucator.incepeRelatieNoua("Ion", "Tata", 100);

    std::cout << "\n--- START VIATA ---" << std::endl;

    // 3. Simulare anuală interactivă
    std::cout << "\n--- INCEPE SIMULAREA ANUALA INTERACTIVA ---" << std::endl;

    for (int i = 0; i < 100; ++i) {  // max 100 ani
        if (jucator.getEsteMort()) break;

        std::cout << "\n================== ANUL " << jucator.getVarsta() << " ==================\n";
        std::cout << jucator << std::endl;

        // 1. Decizie jucător
        jucator.afiseazaMeniuDecizie();
        int alegere = citesteAlegere();

        if (alegere != 0) {
            if (alegere == 1 && jucator.getVarsta() < 18) {
                std::cout << "Nu poti sa te angajezi sau sa studiezi pentru job pana la 18 ani." << std::endl;
            } else if (alegere == 3 && jucator.getVarsta() < 18) {
                std::cout << "Nu poti sa creezi partener sau prieten pana la 18 ani." << std::endl;
            } else {
                jucator.iaDecizieDestin(alegere);
                if (jucator.getEsteMort()) break;
            }
        }

        // 2. Progres anual automat
        bool mort = jucator.aplicaAnual(1);  // aplică un singur an
        if (mort) break;
    }

    std::cout << "\n=================================================" << std::endl;
    std::cout << "  JOC TERMINAT. REZULTAT FINAL." << std::endl;
    std::cout << "=================================================" << std::endl;

    if (!jucator.getEsteMort()) {
        std::cout << jucator << std::endl;
    }

    return 0;
}
