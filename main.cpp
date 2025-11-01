#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <random>
#include <utility>


constexpr int VAL_MIN_STAT = 0;
constexpr int VAL_MAX_STAT = 100;
constexpr int VITEZA_DEGRADARE_FERICIRE = 1;
constexpr int PRAG_SANATATE_CRITICA = 5;
constexpr int MAX_OPTIUNI_DECIZIE = 4; // 1-3 decizii normale + 4. Surrender
constexpr int VARSTA_MAXIMA_FORTATA = 100;

// Date pentru randomizare (40+ Nume, fara diacritice)
const std::vector<std::string> NUME_FEMEI = {
    "Maria", "Elena", "Andreea", "Alexandra", "Ioana", "Diana", "Carmen", "Adina",
    "Ana", "Cristina", "Laura", "Monica", "Alina", "Daniela", "Gabriela", "Roxana",
    "Simona", "Teodora", "Victoria", "Sofia", "Amalia", "Bianca", "Catalina", "Dora",
    "Eliza", "Florentina", "Iulia", "Luminita", "Miruna", "Nicoleta", "Oana", "Paula",
    "Raluca", "Sabina", "Tania", "Valentina", "Yasmina", "Zenobia", "Crina", "Emilia"
};

const std::vector<std::string> NUME_BARBATI = {
    "Alexandru", "Mihai", "Andrei", "Ionut", "Gabriel", "Radu", "Adrian", "Bogdan",
    "Costin", "Daniel", "Emil", "Florin", "George", "Horatiu", "Iulian", "Lucian",
    "Marcel", "Nicolae", "Octavian", "Petru", "Razvan", "Sergiu", "Tudor", "Vlad",
    "Alin", "Ciprian", "Dorin", "Eugen", "Felix", "Horia", "Marius", "Ovidiu",
    "Paul", "Robert", "Sorin", "Valentin", "Victor", "Zian", "Cezar", "Dragos"
};

const std::vector<std::string> STATUS_RELATII = {"Prieten", "Coleg", "Inamic"};

int getRandomInt(const int min, const int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

std::string alegeNumeRandom(const bool eBarbat) {
    if (eBarbat) {
        return NUME_BARBATI[getRandomInt(0, static_cast<int>(NUME_BARBATI.size()) - 1)];
    }
    else {
        return NUME_FEMEI[getRandomInt(0, static_cast<int>(NUME_FEMEI.size()) - 1)];
    }
}

bool citesteDateIntrare(std::string& nume, std::string& prenume, std::string& nationalitate, int& aniSimulare, std::vector<int>& decizii_anuale) {
    std::ifstream fisierTastatura("tastatura.txt");

    decizii_anuale.clear();
    nume = "";
    aniSimulare = 0;

    if (fisierTastatura.is_open()) {
        if (!(fisierTastatura >> nume >> prenume >> nationalitate)) {
             return false;
        }

        if (!(fisierTastatura >> aniSimulare) || aniSimulare < 0) {
             return false;
        }

        int decizie;
        for (int i = 0; i < aniSimulare; ++i) {
            if (fisierTastatura >> decizie) {
                if (decizie >= 0 && decizie <= MAX_OPTIUNI_DECIZIE) {
                    decizii_anuale.push_back(decizie);
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }

        if (decizii_anuale.size() != static_cast<size_t>(aniSimulare)) {
             return false;
        }

        fisierTastatura.close();
        return true;
    }
    else {
        return false;
    }
}

struct DataNastere {
    int zi=0;
    int luna=0;
    const int an = 2025;
};

std::string getLunaString(const int luna) {
    static const std::vector<std::string> numeLuni = {
        "", "Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie",
        "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"
    };
    return numeLuni[luna];
}

DataNastere genereazaDataNastere() {
    DataNastere dn;
    dn.luna = getRandomInt(1, 12);
    int zile_max;
    if (dn.luna == 2) {
        zile_max = 28;
    }
    else
        if (dn.luna == 4 || dn.luna == 6 || dn.luna == 9 || dn.luna == 11) {
            zile_max = 30;
    }
        else {
            zile_max = 31;
        }
    dn.zi = getRandomInt(1, zile_max);
    return dn;
}

std::ostream& operator<<(std::ostream& os, const DataNastere& dn) {
    os << dn.zi << " " << getLunaString(dn.luna) << " " << dn.an;
    return os;
}


class Statistica {
private:
    int valoare; std::string nume;
public:
    Statistica() {
        this->nume = "Necunoscuta";
        this->valoare = 0;
    }
    Statistica(const std::string& nume, int valoare) {
        this->nume = nume;
        this->valoare = valoare;
    }
    Statistica(const Statistica& other) = default;
    Statistica& operator=(const Statistica& other) {
        if (this != &other) {
            this->nume = other.nume;
            this->valoare = other.valoare;
        }
        return *this;
    }
    Statistica(Statistica&& other) = default;
    Statistica& operator=(Statistica&& other) = default;

    [[nodiscard]] int getValoare() const { return valoare; }

    void setValoare(const int nouaValoare) {
        this->valoare = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, nouaValoare));
    }
    friend std::ostream& operator<<(std::ostream& os, const Statistica& s);
};
std::ostream& operator<<(std::ostream& os, const Statistica& s) {
    os << "[" << s.nume << ": " << s.valoare << "]";
    return os;
}


class Statistici {
private:
    Statistica sanatate;
    Statistica fericire;
    Statistica inteligenta;
    Statistica aspect;

    void randomizeStats() {
        this->sanatate = Statistica("Sanatate", getRandomInt(30, 90));
        this->fericire = Statistica("Fericire", getRandomInt(30, 90));
        this->inteligenta = Statistica("Inteligenta", getRandomInt(30, 90));
        this->aspect = Statistica("Aspect", getRandomInt(30, 90));
    }

public:
    Statistici() {randomizeStats();}

    Statistici(const int s,const int f,const int i,const int a) :
        sanatate("Sanatate", s),
        fericire("Fericire", f),
        inteligenta("Inteligenta", i),
        aspect("Aspect", a)
    {}

    Statistici(const Statistici& other) = default;
    Statistici& operator=(const Statistici& other) = default;
    Statistici(Statistici&& other) = default;
    Statistici& operator=(Statistici&& other) = default;

    [[nodiscard]] const Statistica& getInteligenta() const { return inteligenta; }
    [[nodiscard]] const Statistica& getFericire() const { return fericire; }
    [[nodiscard]] const Statistica& getAspect() const { return aspect; }
    [[nodiscard]] const Statistica& getSanatate() const { return sanatate; }

    void modificaStatistica(const std::string& tip, int valoare) {
        if (tip == "Sanatate") {
            sanatate.setValoare(sanatate.getValoare() + valoare);
        }
        else if (tip == "Fericire") {
            fericire.setValoare(fericire.getValoare() + valoare);
        }
        else if (tip == "Inteligenta") {
            inteligenta.setValoare(inteligenta.getValoare() + valoare);
        }
        else if (tip == "Aspect") {
            aspect.setValoare(aspect.getValoare() + valoare);
        }
    }

    [[nodiscard]] bool areStatisticiSanatoase() const {
        constexpr int PRAG_SANATATE_BINE = 50;
        return sanatate.getValoare() > PRAG_SANATATE_BINE && fericire.getValoare() > PRAG_SANATATE_BINE;
    }
    friend std::ostream& operator<<(std::ostream& os, const Statistici& s);
};
std::ostream& operator<<(std::ostream& os, const Statistici& s) {os << s.sanatate << " " << s.fericire << " " << s.inteligenta << " " << s.aspect; return os;}


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

    int incearcaPromovare(const int inteligentaPersonaj) {
        const int sansaBaza = (inteligentaPersonaj - cerintaInteligenta) / 2;
        const int sansaFinala = std::max(10, std::min(90, sansaBaza + getRandomInt(-10, 10)));
        std::cout << "* PROMOVARE: Sansa de succes: " << sansaFinala << "%. ";
        if (getRandomInt(1, 100) <= sansaFinala) {
            salariuAnual += 20;
            satisfactie += 15;
            ajusteazaSatisfactia(satisfactie);
            std::cout << "SUCCES! Salariu: +20, Satisfactie: +15." << std::endl;
            return 1;
        }
        else {
            std::cout << "ESEC. Nu esti inca pregatit." << std::endl;
            return 0;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Cariera& c);
};
std::ostream& operator<<(std::ostream& os, const Cariera& c) {
    os << "Job: " << c.numeJob << " | Salariu: " << c.salariuAnual << "K | Satisfactie: " << c.satisfactie << "%";
    return os;
}

class Relatie {
private:
    std::string numePersoana;
    std::string tipRelatie;
    int nivelAfectiune;

public:
    Relatie(const std::string& nume, const std::string& tip, int afectiune) {
        this->numePersoana = nume;
        this->tipRelatie = tip;
        this->nivelAfectiune = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, afectiune));
    }
    Relatie(const Relatie& other) {
        this->numePersoana = other.numePersoana;
        this->tipRelatie = other.tipRelatie;
        this->nivelAfectiune = other.nivelAfectiune;
        std::cout << "[LOG: Relatie] Constructor de copiere apelat pentru relatie cu " << numePersoana << std::endl;
    }
    Relatie& operator=(const Relatie& other) {
        if (this != &other) {
            this->numePersoana = other.numePersoana;
            this->tipRelatie = other.tipRelatie;
            this->nivelAfectiune = other.nivelAfectiune;
            std::cout << "[LOG: Relatie] Operator= de copiere apelat pentru relatie cu " << numePersoana << std::endl;
        }
        return *this;
    }
    ~Relatie() {
    std::cout << "[LOG: Relatie] Destructor apelat pentru relatie cu " << numePersoana << std::endl;
    }

    [[nodiscard]] int getNivelAfectiune() const { return nivelAfectiune; }
    [[nodiscard]] const std::string& getNumePersoana() const { return numePersoana; }
    [[nodiscard]] const std::string& getTipRelatie() const { return tipRelatie; }

    void imbunatatesteRelatia(int puncte) {
        nivelAfectiune = std::min(VAL_MAX_STAT, nivelAfectiune + puncte);
    }

    friend std::ostream& operator<<(std::ostream& os, const Relatie& r);
};

std::ostream& operator<<(std::ostream& os, const Relatie& r) {
    os << r.tipRelatie << " (" << r.numePersoana << ") - Afectiune: " << r.nivelAfectiune;
    return os;
}

class EvenimentViata {
private:
    std::string descriere;
    std::string tipImpact;
    int valoareImpact;
public:
    EvenimentViata(const std::string& desc, const std::string& tip, const int val) {
        this->descriere = desc;
        this->tipImpact = tip;
        this->valoareImpact = val;
    }
    friend std::ostream& operator<<(std::ostream& os, const EvenimentViata& e);
};
std::ostream& operator<<(std::ostream& os, const EvenimentViata& e) {
    os << e.descriere << " (Impact: " << e.tipImpact << " de " << e.valoareImpact << ")";
    return os;
}

struct Job {
    std::string nume;
    int salariuAnual;
    int satisfactie;
    int cerintaInteligenta;
};

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
    static constexpr int MAX_RELATII = 5;

    void adaugaRelatieIntern(const Relatie& r) {
        if (relatii.size() < MAX_RELATII) {
            relatii.push_back(r);
        }
        else {
            std::cout << "[LOG] Limita de relatii atinsa." << std::endl;
        }
    }

    void creeazaRelatieRandom() {
        if (relatii.size() < MAX_RELATII) {
            const bool eBarbat = (getRandomInt(0, 1) == 0);
            const std::string& nume = alegeNumeRandom(eBarbat);
            const std::string& status = STATUS_RELATII[getRandomInt(0, static_cast<int>(STATUS_RELATII.size()) - 1)];
            int afectiune = getRandomInt(20, 95);

            int impactFericire = 0;
            int impactSanatate = 0;

            if (status == "Prieten") {
                impactFericire = 15;
                impactSanatate = 10;
            }
            else
                if (status == "Coleg") {
                    impactFericire = 5;
                    impactSanatate = 0;
                    afectiune = getRandomInt(40, 75);
                }
                else
                    if (status == "Inamic") {
                        impactFericire = -15;
                        impactSanatate = -5;
                        afectiune = getRandomInt(0, 30);
                    }

            const Relatie r(nume, status, afectiune);
            adaugaRelatieIntern(r);

            stats.modificaStatistica("Fericire", impactFericire);
            stats.modificaStatistica("Sanatate", impactSanatate);

            std::cout << "Noua relatie: " << status << " cu " << nume << " (Afectiune: " << afectiune <<
                    ").  Impact Stat: Fericire " << (impactFericire > 0 ? "+" : "") << impactFericire << ", Sanatate "
                    << (impactSanatate > 0 ? "+" : "") << impactSanatate << "." << std::endl;
        }
    }

    void marcheazaDeces(const std::string& cauza) {
        if (esteMort) {
            return;
        }
        esteMort = true;
        std::cout << "=================================================" << std::endl;
        std::cout << "=== JOC TERMINAT! Jucatorul a murit la " << varsta << " ani. ===" << std::endl;
        std::cout << "=== Cauza: " << cauza << " ===" << std::endl;
        std::cout << "=================================================" << std::endl;
    }

    bool verificaDeces() {
        if (esteMort)
            return true;
        if (stats.getSanatate().getValoare() < PRAG_SANATATE_CRITICA) {
            marcheazaDeces("Sanatate extrem de scazuta");
            return true;
        }
        return false;
    }

    void evenimentAleatoriu() {
        const int fericireInitiala = stats.getFericire().getValoare();
        const double baniInitiali = bani; // Salvam banii pentru log

        if (const int sansa = getRandomInt(1, 100); sansa < 35) {
            const int impact = getRandomInt(-15, 20);
            std::string descriere = "S-a intamplat un eveniment minor in viata ta.";

            if (impact < 0) {
                descriere = "Ai pierdut un obiect valoros.";
                stats.modificaStatistica("Fericire", impact);
            }
            else
                if (impact > 10) {
                    descriere = "Ai gasit 50 de lei pe strada!";
                    bani += 0.05;
                    stats.modificaStatistica("Fericire", impact);
                }
                else {
                    stats.modificaStatistica("Fericire", impact);
                }

            std::cout << "* Eveniment Aleatoriu: " << descriere << std::endl;

            if (getRandomInt(1, 10) > 8) {
                creeazaRelatieRandom();
            }

            const int fericireFinala = stats.getFericire().getValoare();
            const double baniFinali = bani;

            const int diferentaFericire = fericireFinala - fericireInitiala;
            const double diferentaBani = baniFinali - baniInitiali;

            std::cout << "  - Status Fericire: " << (diferentaFericire > 0 ? "+" : "") << diferentaFericire << std::endl;

            if (diferentaBani != 0.0) {
                 std::cout << "  - BANI: " << (diferentaBani > 0.0 ? "+" : "") << std::fixed << std::setprecision(2) << diferentaBani << "K" << std::endl;
            }
        }
    }

    void obtinePrimulJob() {
        const int inteligenta = stats.getInteligenta().getValoare();
        const std::vector<Job> joburiDisponibile = {
            {"Lucrator in Depozit", 20, 30, 0},
            {"Vanzator in Magazin", 35, 40, 35},
            {"Asistent Administrativ", 50, 55, 60},
            {"Programator Junior", 80, 70, 80},
            {"Cercetator Stiintific", 120, 85, 95}
        };

        Job jobAles = joburiDisponibile[0];

        for (const auto& job : joburiDisponibile) {
            if (inteligenta >= job.cerintaInteligenta) {
                jobAles = job;
            }
        }

        cariera = Cariera(jobAles.nume, jobAles.salariuAnual, jobAles.satisfactie, jobAles.cerintaInteligenta);

        std::cout << "\n=================================================" << std::endl;
        std::cout << ">>> FELICITARI! AI IMPLINIT 18 ANI! <<<" << std::endl;
        std::cout << numeComplet << ", datorita inteligentei tale de " << inteligenta
                  << ", ai obtinut primul tau loc de munca:    " << std::endl;
        std::cout << cariera.getNumeJob() << " (Salariu Anual: " << cariera.getSalariuAnual() << "K) <<<" << std::endl;
        std::cout << "=================================================" << std::endl;
    }

    void intretinereFinanciara() {
        double costViata = 0.0;
        if (varsta >= 18) {
            costViata = 10.0;
        }

        const double baniInitiali = bani;
        bani += cariera.getSalariuAnual();
        bani -= costViata;

        std::cout << "* FINANCIAR: Salariu primit (+"<< cariera.getSalariuAnual() <<"K), Costuri de trai (-" << costViata << "K)." << std::endl;

        if (bani < 0 && baniInitiali >= 0.0) {
            std::cout << "--- IMPACT DATORII ---" << std::endl;
            std::cout << "[! ATENTIE] AI INTRAT IN DATORII! Fericirea scade cu -10." << std::endl;
            stats.modificaStatistica("Fericire", -10);
            std::cout << "----------------------" << std::endl;
        }
        else
            if (bani < 0) {
            std::cout << "[! ATENTIE] Esti inca in datorii." << std::endl;
            }
    }


    void gestioneazaExpeditiePericuloasa() {
        std::cout << ">> Pregatirea pentru eveniment. Verificam riscurile..." << std::endl;

        const int sanatateInitiala = stats.getSanatate().getValoare();
        const int fericireInitiala = stats.getFericire().getValoare();
        const int inteligentaInitiala = stats.getInteligenta().getValoare();
        const int aspectInitial = stats.getAspect().getValoare();
        const bool pregatit = stats.areStatisticiSanatoase();

        struct Scenariu {
            std::string descriere;
            int impactSanatate;
            int impactFericire;
            int impactInteligenta;
            int impactAspect;
        };

        const std::vector<Scenariu> scenarii = {
            {"Ai fost pe o drumetie montana si te-ai ratacit. Ai pierdut o zi, dar ai invatat sa te descurci.",
                -5, 0, +15, 0},
            {"Ai asistat la un accident si ai ajutat la salvarea unei vieti. Esti epuizat, dar fericit.",
                -10, +25, 0, 0},
            {"Ai fost atacat de un animal salbatic. Ai scapat, dar ai suferit rani vizibile.",
                -30, -10, 0, -20},
            {"Ai descoperit o pestera veche cu picturi rupestre. Sanatatea nu este afectata, dar intelectul creste.",
                0, +10, +20, 0},
            // Scenariu 4: Vreme rea si izolare (Risc Minor)
            {"Vremea te-a prins nepregatit. Ai rezistat cu greu. Emotional, esti daramat.",
                0, -15, 0, 0}
        };

        const int indexScenariu = getRandomInt(0, static_cast<int>(scenarii.size()) - 1);
        const Scenariu& s = scenarii[indexScenariu];

        std::cout << "PROMPT: " << s.descriere << std::endl;

        stats.modificaStatistica("Sanatate", s.impactSanatate);
        stats.modificaStatistica("Fericire", s.impactFericire);
        stats.modificaStatistica("Inteligenta", s.impactInteligenta);
        stats.modificaStatistica("Aspect", s.impactAspect);

        if (const int sansaNoroc = getRandomInt(1, 100); !pregatit && sansaNoroc < 50) {
            std::cout << "REZULTAT FINAL: Esec critic din lipsa de pregatire! Penalizari suplimentare aplicate." << std::endl;
            stats.modificaStatistica("Sanatate", -20);
            stats.modificaStatistica("Fericire", -15);
        }
        else
            if (pregatit && sansaNoroc > 80) {
                std::cout << "REZULTAT FINAL: Echipament excelent si noroc! Ai primit un bonus de fericire." << std::endl;
                stats.modificaStatistica("Fericire", +10);
            }

        std::cout << "\n--- REZUMAT IMPACT EXPEDITIE ---" << std::endl;

        auto afiseazaImpact = [&](const std::string& nume, const int initial, const int final) {
            if (const int diferenta = final - initial; diferenta != 0) {
                std::cout << " * " << nume << " " << (diferenta > 0 ? "+" : "") << diferenta << std::endl;
            }
        };

        afiseazaImpact("Sanatate", sanatateInitiala, stats.getSanatate().getValoare());
        afiseazaImpact("Fericire", fericireInitiala, stats.getFericire().getValoare());
        afiseazaImpact("Inteligenta", inteligentaInitiala, stats.getInteligenta().getValoare());
        afiseazaImpact("Aspect", aspectInitial, stats.getAspect().getValoare());
        std::cout << "--------------------------------\n" << std::endl;
    }


public:
    static void afiseazaMeniuDecizie() {
        std::cout << "\n--- OPTIUNI ANUALE (CITITE DIN FISIER) ---" << std::endl;
        std::cout << "0. Continua la Anul Urmator (Fara Decizie majora)" << std::endl;
        std::cout << "1. Incearca Promovarea/Studiu (Inteligenta)" << std::endl;
        std::cout << "2. Expeditie Periculoasa (Eveniment Major cu Scenarii)" << std::endl;
        std::cout << "3. Creeaza o noua relatie (Prieten/Coleg/Inamic)" << std::endl;
        std::cout << "4. RENUNTA / SURRENDER (Termina jocul)" << std::endl;
    }

    Personaj(const std::string& nume, const std::string& nat, const int varsta, const DataNastere& dn, Statistici initialStats)
    : dataNastere(dn), stats(std::move(initialStats))
    {
        this->numeComplet = nume;
        this->nationalitate = nat;
        this->varsta = varsta;
        this->bani = 0.0;
        this->varstaDecesAleatorie = getRandomInt(70, 99);
        this->esteMort = false;
        std::cout << "[LOG] Varsta de deces natural setata la " << this->varstaDecesAleatorie << " ani." << std::endl;
    }

    Personaj(const std::string& nume, const std::string& nat, const int varsta, const DataNastere& dn)
        : Personaj(nume, nat, varsta, dn, Statistici())
    {}

    [[nodiscard]] const std::string& getNume() const { return numeComplet; }
    [[nodiscard]] int getVarsta() const { return varsta; }

    [[nodiscard]] bool aplicaAnual(const int ani = 1) {
        if (esteMort) return true;

        for (int i = 0; i < ani; ++i) {
            if (varsta >= varstaDecesAleatorie || varsta >= VARSTA_MAXIMA_FORTATA) {
                marcheazaDeces("Batranete");
                return true;
            }
            varsta += 1;
            if (varsta == 18) {
                obtinePrimulJob();
            }

            std::cout << "\n=================================================" << std::endl;
            std::cout << "--- START ANUL " << varsta << " | " << numeComplet << " ---" << std::endl;
            std::cout << "=================================================" << std::endl;

            stats.modificaStatistica("Fericire", -VITEZA_DEGRADARE_FERICIRE);
            std::cout << "* Degradare Fericire: -" << VITEZA_DEGRADARE_FERICIRE << std::endl;

            if (verificaDeces()) return true;
            intretinereFinanciara();
            if (verificaDeces()) return true;
            evenimentAleatoriu();
            if (verificaDeces()) return true;

            std::cout << "\n--- IMPACT ANUAL RELATII ---" << std::endl;
            for (Relatie& r : relatii) {
                r.imbunatatesteRelatia(-2);

                if (const int nivelAfectiune = r.getNivelAfectiune(); r.getTipRelatie() == "Prieten" && nivelAfectiune > 70) {
                    stats.modificaStatistica("Fericire", 3);
                    stats.modificaStatistica("Sanatate", 1);
                    std::cout << "* " << r.getNumePersoana() << " (Prieten Afec: " << nivelAfectiune << ") iti aduce bucurie: Fericire +3, Sanatate +1." << std::endl;
                }
                else
                    if (r.getTipRelatie() == "Inamic" && nivelAfectiune < 30) {
                        stats.modificaStatistica("Fericire", -5);
                        stats.modificaStatistica("Sanatate", -2);
                        std::cout << "* " << r.getNumePersoana() << " (Inamic Afec: " << nivelAfectiune << ") te streseaza: Fericire -5, Sanatate -2." << std::endl;
                    }
                    else
                        if (r.getTipRelatie() == "Mama" || r.getTipRelatie() == "Tata") {
                            stats.modificaStatistica("Fericire", 1);
                            std::cout << "* " << r.getNumePersoana() << " (" << r.getTipRelatie() << ") iti ofera un confort constant: Fericire +1." << std::endl;
                        }
            }
            std::cout << "----------------------------" << std::endl;
        }
        return esteMort;
    }

    void incepeRelatieNoua(const std::string& nume, const std::string& tip, const int afectiune) {
        if (esteMort) { return; }
        const Relatie r(nume, tip, afectiune);
        adaugaRelatieIntern(r);
    }

    void iaDecizieDestin(const int alegere) {
        if (esteMort) return;
        std::cout << "--- EXECUT DECZIA ANUALA: " << alegere << " ---" << std::endl;
        int impactFericire = 0;

        switch (alegere) {
            case 1: {
                std::cout << "Actiune: Incearca Promovarea. ";
                if (cariera.getNumeJob() != "Somaj") {
                    cariera.incearcaPromovare(stats.getInteligenta().getValoare());
                    impactFericire += 10;
                }
                else {
                    std::cout << "Nu ai job. Ai studiat degeaba. ";
                    stats.modificaStatistica("Inteligenta", 5);
                }
                break;
            }
            case 2: {
                std::cout << "Actiune: Initierea unei expeditii periculoase..." << std::endl;
                gestioneazaExpeditiePericuloasa();
                break;
            }
            case 3: {
                std::cout << "Actiune: Intalnesti pe cineva nou. ";
                creeazaRelatieRandom();
                impactFericire += 15;
                break;
            }
            case 4: {
                std::cout << "Actiune: Jucatorul a renuntat la viata!" << std::endl;
                marcheazaDeces("Renuntare (Surrender)");
                break;
            }
            case 0: {
                std::cout << "Actiune: Nicio decizie majora luata. Se trece la anul urmator." << std::endl;
                break;
            }
            default: {
                std::cout << "Alegere invalida. Nicio actiune." << std::endl;
            }
        }

        if (impactFericire != 0)
            stats.modificaStatistica("Fericire", impactFericire);

        verificaDeces();
    }

    [[nodiscard]] bool getEsteMort() const {return esteMort;}

    friend std::ostream& operator<<(std::ostream& os, const Personaj& p);
};

std::ostream& operator<<(std::ostream& os, const Personaj& p) {
    os << "--- " << p.numeComplet << " (" << p.varsta << " ani, " << p.nationalitate << ") ---" << "\n";
    os << " Data Nastere: " << p.dataNastere << "\n";
    os << " BANI: " << std::fixed << std::setprecision(2) << p.bani << "K | ";
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


int main() {
    std::cout << "--- BITLIFE: DESTINY SIMULATOR v4.0 (MOD BATCH STRICT) ---" << std::endl;

    std::string nume, prenume, nationalitate;
    int ani_simulare = 0;
    std::vector<int> decizii_anuale;

    if (!citesteDateIntrare(nume, prenume, nationalitate, ani_simulare, decizii_anuale) || ani_simulare == 0) {
        std::cout << "\nNu s-a putut realiza citirea datelor de intrare. Parasirea jocului..." << std::endl;
        std::cout << "\n=================================================" << std::endl;
        std::cout << "  JOC TERMINAT. REZULTAT FINAL." << std::endl;
        std::cout << "=================================================" << std::endl;

        return 0;
    }

    std::string numeComplet = nume + " " + prenume;
    DataNastere dn = genereazaDataNastere();

    constexpr int MIN_HIGH_STAT = 50;
    constexpr int MAX_HIGH_STAT = 95;

    int s = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);
    int f = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);
    int i = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);
    int a = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);

    Statistici stats_initiale(s, f, i, a); // Apel la constructorul cu parametri
    std::cout << "\n[TEST LOG] Statistici initiale setate: " << stats_initiale << std::endl;
    std::cout << "\n=================================================" << std::endl;
    std::cout << "Numele personajului: " << numeComplet << std::endl;
    std::cout << "Nationalitatea: " << nationalitate << std::endl;
    std::cout << "Data de nastere: " << dn << std::endl;
    std::cout << "Ani de Simulat (din fisier): " << ani_simulare << std::endl;
    std::cout << "Decizii folosite: ";
    for (size_t k = 0; k < decizii_anuale.size(); ++k) {
        std::cout << decizii_anuale[k] << (k < decizii_anuale.size() - 1 ? ", " : "");
    }
    std::cout << "\n=================================================" << std::endl;

    Personaj jucator(numeComplet, nationalitate, 1, dn, stats_initiale);

    std::cout << "\n[TEST LOG] 2. Testam Constructorul de Copiere al Relatie (Parinti cu nume specifice):" << std::endl;

    std::string nume_mama = alegeNumeRandom(false);
    jucator.incepeRelatieNoua(nume_mama, "Mama", 100);
    std::string nume_tata = alegeNumeRandom(true);
    jucator.incepeRelatieNoua(nume_tata, "Tata", 100);

    std::cout << "\n[TEST LOG] 3. Testam Operator= de Copiere al Relatie:" << std::endl;
    Relatie relatie_originala("TestOp", "Prieten", 70);
    Relatie relatie_noua("Inlocuit", "Necunoscut", 10);

    relatie_noua = relatie_originala;

    std::cout << "\n[TEST LOG] Relatia 'relatie_noua' dupa Op=: " << relatie_noua << std::endl;
    std::cout << "\n--- START VIATA ---" << std::endl;

    for (int i1 = 0; i1 < ani_simulare; ++i1) {
        if (jucator.getEsteMort()) break;

        std::cout << "\n--- STARE CURENTA INAINTE DE DECIZIA ANULUI " << jucator.getVarsta() + 1 << " ---" << std::endl;
        std::cout << jucator << std::endl;

        int alegere = decizii_anuale[i1];

        Personaj::afiseazaMeniuDecizie();
        std::cout << "\n> ACTIUNE JUCATOR (DIN FISIER): Se executa decizia " << alegere << " pentru anul " << jucator.getVarsta() + 1 << "..." << std::endl;

        jucator.iaDecizieDestin(alegere);
        if (jucator.getEsteMort()) break;

        if (jucator.aplicaAnual(1)) break;
    }

    std::cout << "\n=================================================" << std::endl;
    std::cout << "  JOC TERMINAT. REZULTAT FINAL." << std::endl;
    std::cout << "=================================================" << std::endl;

    if (!jucator.getEsteMort()) {
        std::cout << jucator << std::endl;
    }
    return 0;
}