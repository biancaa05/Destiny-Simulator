#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <random>
#include <utility> // Pentru std::move

// ====================================================================
// GRUP 1: CONSTANTE GLOBALE SI FUNCTII NON-MEMBRU DE UTILITATE (HELPERS)
// ====================================================================

// Constante globale pentru joc
const int VAL_MIN_STAT = 0;
const int VAL_MAX_STAT = 100;
const int VITEZA_DEGRADARE_FERICIRE = 1;
const int PRAG_SANATATE_CRITICA = 5;
const int MAX_OPTIUNI_DECIZIE = 4; // 1-3 decizii normale + 4. Surrender
const int VARSTA_MAXIMA_FORTATA = 100;

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

// Functie helper pentru generarea numerelor aleatoare
int getRandomInt(int min, const int max) {
    static std::random_device rd;      // seed hardware
    static std::mt19937 gen(rd());     // generator Mersenne Twister
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Functie helper pentru a alege un nume aleatoriu dintr-o lista specifica
std::string alegeNumeRandom(const bool eBarbat) {
    if (eBarbat) {
        return NUME_BARBATI[getRandomInt(0, static_cast<int>(NUME_BARBATI.size()) - 1)];
    } else {
        return NUME_FEMEI[getRandomInt(0, static_cast<int>(NUME_FEMEI.size()) - 1)];
    }
}


// Functie helper pentru citirea si validarea deciziei
int citesteAlegere() {
    int alegere = 0;
    std::cout << "\n> ACTIUNE JUCATOR: Introduceti numarul deciziei (1-" << MAX_OPTIUNI_DECIZIE << ", 4. Surrender, sau 0 pentru a inainta timpul): ";

    while (!(std::cin >> alegere) || alegere < 0 || alegere > MAX_OPTIUNI_DECIZIE) {
        std::cout << "Optiune invalida. Va rugam introduceti un numar valid (0-" << MAX_OPTIUNI_DECIZIE << "): ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return alegere;
}

// Functie pentru citirea datelor initiale
void citesteDateIntrare(std::string& nume, std::string& prenume, std::string& nationalitate) {
    std::ifstream fisierTastatura("tastatura.txt");
    if (fisierTastatura.is_open()) {
        if (fisierTastatura >> nume >> prenume >> nationalitate) {
            // Success
        } else {
             std::cout << "Eroare citire date din tastatura.txt. Folosind date implicite." << std::endl;
             nume = "Andrei";
             prenume = "Popescu";
             nationalitate = "Roman";
        }
        fisierTastatura.close();
    } else {
        std::cout << "Eroare la deschiderea fisierului tastatura.txt! Folosind date implicite." << std::endl;
        nume = "Andrei";
        prenume = "Popescu";
        nationalitate = "Roman";
    }
}


// ====================================================================
// GRUP 2: STRUCTURI SI CLASE (DATA SI STATISTICI)
// ====================================================================

// Structura si Functii pentru Data de Nastere
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
    if (dn.luna == 2) {zile_max = 28;}
    else if (dn.luna == 4 || dn.luna == 6 || dn.luna == 9 || dn.luna == 11) {zile_max = 30;}
    else {zile_max = 31;}
    dn.zi = getRandomInt(1, zile_max);
    return dn;
}

// Operator<< pentru afisarea datei
std::ostream& operator<<(std::ostream& os, const DataNastere& dn) {
    os << dn.zi << " " << getLunaString(dn.luna) << " " << dn.an;
    return os;
}


// CLASA 1: STATISTICA (CORECTATA pentru Rule of Five)
class Statistica {
private:
    int valoare; std::string nume;
public:
    Statistica() {this->nume = "Necunoscuta"; this->valoare = 0;}
    Statistica(const std::string& nume, int valoare) {this->nume = nume; this->valoare = valoare;}

    // NOU: Membrii adaugati pentru a rezolva eroarea Clang/G++ strict
    Statistica(const Statistica& other) = default; // C. de Copiere
    Statistica& operator=(const Statistica& other) { // Op= de Copiere (existenta)
        if (this != &other) {this->nume = other.nume; this->valoare = other.valoare;} return *this;}
    Statistica(Statistica&& other) = default; // C. de Mutare
    Statistica& operator=(Statistica&& other) = default; // Op= de Mutare
    // (Destructorul este implicit si OK)

    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] int getValoare() const { return valoare; }
    void setValoare(int nouaValoare) {
        this->valoare = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, nouaValoare));
    }
    [[nodiscard]] bool esteCritica() const { return valoare < PRAG_SANATATE_CRITICA; }
    friend std::ostream& operator<<(std::ostream& os, const Statistica& s);
};
std::ostream& operator<<(std::ostream& os, const Statistica& s) {os << "[" << s.nume << ": " << s.valoare << "]"; return os;}


// CLASA 2: STATISTICI (CORECTATA pentru Rule of Five)
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
    // 1. Constructor Implicit
    Statistici() {randomizeStats();}

    // 2. Constructor de initializare cu parametri
    Statistici(int s, int f, int i, int a) :
        sanatate("Sanatate", s),
        fericire("Fericire", f),
        inteligenta("Inteligenta", i),
        aspect("Aspect", a)
    {}

    // NOU: Membrii adaugati pentru a rezolva eroarea G++ strict
    Statistici(const Statistici& other) = default;
    Statistici& operator=(const Statistici& other) = default;
    Statistici(Statistici&& other) = default;
    Statistici& operator=(Statistici&& other) = default;

    [[nodiscard]] const Statistica& getInteligenta() const { return inteligenta; }
    [[nodiscard]] const Statistica& getFericire() const { return fericire; }
    [[nodiscard]] const Statistica& getAspect() const { return aspect; }
    [[nodiscard]] const Statistica& getSanatate() const { return sanatate; }

    void modificaStatistica(const std::string& tip, int valoare) {
        if (tip == "Sanatate") {sanatate.setValoare(sanatate.getValoare() + valoare);}
        else if (tip == "Fericire") {fericire.setValoare(fericire.getValoare() + valoare);}
        else if (tip == "Inteligenta") {inteligenta.setValoare(inteligenta.getValoare() + valoare);}
        else if (tip == "Aspect") {aspect.setValoare(aspect.getValoare() + valoare);}}

    [[nodiscard]] bool areStatisticiSanatoase() const {
        const int PRAG_SANATATE_BINE = 50;
        return sanatate.getValoare() > PRAG_SANATATE_BINE && fericire.getValoare() > PRAG_SANATATE_BINE;
    }
    friend std::ostream& operator<<(std::ostream& os, const Statistici& s);
};
std::ostream& operator<<(std::ostream& os, const Statistici& s) {os << s.sanatate << " " << s.fericire << " " << s.inteligenta << " " << s.aspect; return os;}


// ====================================================================
// GRUP 3: CLASELE SECUNDARE (CARIERA, RELATIE, EVENIMENT)
// ====================================================================

// CLASA 3: CARIERA
class Cariera {
private:
    std::string numeJob;
    int salariuAnual; // Salariu in K (Mii)
    int satisfactie;
    int cerintaInteligenta;
    void ajusteazaSatisfactia(int val) {
        satisfactie = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, val));
    }
public:
    Cariera() {
        this->numeJob = "Somaj"; this->salariuAnual = 0; this->satisfactie = 10; this->cerintaInteligenta = 0;
    }
    Cariera(const std::string& nume, int salariu, int satisf, int cerinta) {
        this->numeJob = nume; this->salariuAnual = salariu; this->satisfactie = satisf; this->cerintaInteligenta = cerinta;
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


// CLASA 4: RELATIE (Simplificata)
class Relatie {
private:
    std::string numePersoana;
    std::string tipRelatie;
    int nivelAfectiune;

public:
    // Constructor obisnuit (cu parametri)
    Relatie(const std::string& nume, const std::string& tip, int afectiune) {
        this->numePersoana = nume;
        this->tipRelatie = tip;
        this->nivelAfectiune = std::max(VAL_MIN_STAT, std::min(VAL_MAX_STAT, afectiune));
    }

    // Constructor de copiere (cu log)
    Relatie(const Relatie& other) {
        this->numePersoana = other.numePersoana;
        this->tipRelatie = other.tipRelatie;
        this->nivelAfectiune = other.nivelAfectiune;
        std::cout << "[LOG: Relatie] Constructor de copiere apelat pentru relatie cu " << numePersoana << std::endl;
    }

    // Operator= de copiere (cu log)
    Relatie& operator=(const Relatie& other) {
        if (this != &other) {
            this->numePersoana = other.numePersoana;
            this->tipRelatie = other.tipRelatie;
            this->nivelAfectiune = other.nivelAfectiune;
            std::cout << "[LOG: Relatie] Operator= de copiere apelat pentru relatie cu " << numePersoana << std::endl;
        }
        return *this;
    }

    // Destructor (cu log)
    ~Relatie() {
    std::cout << "[LOG: Relatie] Destructor apelat pentru relatie cu " << numePersoana << std::endl;
    }

    [[nodiscard]] int getNivelAfectiune() const {
        return nivelAfectiune;
    }

    [[nodiscard]] const std::string& getNumePersoana() const { return numePersoana; }
    [[nodiscard]] const std::string& getTipRelatie() const { return tipRelatie; }

    void imbunatatesteRelatia(int puncte) {
        nivelAfectiune = std::min(VAL_MAX_STAT, nivelAfectiune + puncte);
    }

    friend std::ostream& operator<<(std::ostream& os, const Relatie& r);
};

// Operator << pentru Relatie
std::ostream& operator<<(std::ostream& os, const Relatie& r) {
    os << r.tipRelatie << " (" << r.numePersoana << ") - Afectiune: " << r.nivelAfectiune;
    return os;
}

// CLASA 5: EVENIMENTVIATA
class EvenimentViata {
private: std::string descriere; std::string tipImpact; int valoareImpact;
public:
    EvenimentViata(const std::string& desc, const std::string& tip, int val) {
        this->descriere = desc; this->tipImpact = tip; this->valoareImpact = val;}
    friend std::ostream& operator<<(std::ostream& os, const EvenimentViata& e);
};
std::ostream& operator<<(std::ostream& os, const EvenimentViata& e) {
    os << e.descriere << " (Impact: " << e.tipImpact << " de " << e.valoareImpact << ")"; return os;}


// ====================================================================
// GRUP 4: CLASA PRINCIPALA (PERSONAJ)
// ====================================================================

// Structura interna pentru a defini joburile
struct Job {
    std::string nume;
    int salariuAnual; // K (Mii)
    int satisfactie;
    int cerintaInteligenta;
};

class Personaj {
private:
    std::string numeComplet;
    std::string nationalitate;
    DataNastere dataNastere;
    int varsta;
    double bani; // In K (Mii de lei/valuta)
    int varstaDecesAleatorie;
    bool esteMort;
    Statistici stats; // Obiect de compunere
    Cariera cariera;
    std::vector<Relatie> relatii;
    static const int MAX_RELATII = 5;

    void adaugaRelatieIntern(const Relatie& r) {
        if (relatii.size() < MAX_RELATII) {relatii.push_back(r);} else {std::cout << "[LOG] Limita de relatii atinsa." << std::endl;} }

    void creeazaRelatieRandom() {
        if (relatii.size() < MAX_RELATII) {
            // Alegerea unui nume aleatoriu (50% sansa de a fi barbat, 50% femeie)
            bool eBarbat = (getRandomInt(0, 1) == 0);
            const std::string& nume = alegeNumeRandom(eBarbat);

            const std::string& status = STATUS_RELATII[getRandomInt(0, static_cast<int>(STATUS_RELATII.size()) - 1)];
            int afectiune = getRandomInt(20, 95);

            int impactFericire = 0;
            int impactSanatate = 0;

            // Logica de ajustare a afectiunii si a impactului statistic
            if (status == "Prieten") {
                impactFericire = 15;
                impactSanatate = 10;
            } else if (status == "Coleg") {
                impactFericire = 5;
                impactSanatate = 0;
                afectiune = getRandomInt(40, 75);
            } else if (status == "Inamic") {
                impactFericire = -15;
                impactSanatate = -5;
                afectiune = getRandomInt(0, 30);
            }

            Relatie r(nume, status, afectiune);
            adaugaRelatieIntern(r);

            // Aplicarea impactului statisticilor
            stats.modificaStatistica("Fericire", impactFericire);
            stats.modificaStatistica("Sanatate", impactSanatate);

            std::cout << "Noua relatie: " << status << " cu " << nume
                      << " (Afectiune: " << afectiune << ")."
                      << " Impact Stat: Fericire " << (impactFericire > 0 ? "+" : "") << impactFericire
                      << ", Sanatate " << (impactSanatate > 0 ? "+" : "") << impactSanatate << "." << std::endl;
        }
    }

    // Corectie indentare: se adauga acolade
    void marcheazaDeces(const std::string& cauza) {
        if (esteMort) { return; } esteMort = true;
        std::cout << "\n=================================================" << std::endl;
        std::cout << "=== JOC TERMINAT! Jucatorul a murit la " << varsta << " ani. ===" << std::endl;
        std::cout << "=== Cauza: " << cauza << " ===" << std::endl;
        std::cout << "=================================================" << std::endl;
    }

    bool verificaDeces() {
        if (esteMort) return true;
        if (stats.getSanatate().getValoare() < PRAG_SANATATE_CRITICA) {
            marcheazaDeces("Sanatate extrem de scazuta"); return true; }
        return false;
    }

    // FUNCTIE EVENIMENT ALEATORIU MODIFICATA
    void evenimentAleatoriu() {
        // Salvarea starii initiale
        int fericireInitiala = stats.getFericire().getValoare();
        double baniInitiali = bani; // Salvam banii pentru log

        int sansa = getRandomInt(1, 100);
        if (sansa < 35) {
            int impact = getRandomInt(-15, 20);
            std::string descriere = "S-a intamplat un eveniment minor in viata ta.";

            // Logica de baza
            if (impact < 0) {
                descriere = "Ai pierdut un obiect valoros.";
                stats.modificaStatistica("Fericire", impact);
            } else if (impact > 10) {
                descriere = "Ai gasit 50 de lei pe strada!";
                bani += 0.05;
                stats.modificaStatistica("Fericire", impact);
            } else {
                stats.modificaStatistica("Fericire", impact);
            }

            std::cout << "* Eveniment Aleatoriu: " << descriere << std::endl;

            if (getRandomInt(1, 10) > 8) {
                creeazaRelatieRandom();
            }

            // NOU: Afisarea Impactului Final al Evenimentului Aleatoriu
            int fericireFinala = stats.getFericire().getValoare();
            double baniFinali = bani;

            int diferentaFericire = fericireFinala - fericireInitiala;
            double diferentaBani = baniFinali - baniInitiali;

            std::cout << "  - Status Fericire: " << (diferentaFericire > 0 ? "+" : "")
                      << diferentaFericire << std::endl;

            if (diferentaBani != 0.0) {
                 std::cout << "  - BANI: " << (diferentaBani > 0.0 ? "+" : "")
                           << std::fixed << std::setprecision(2) << diferentaBani << "K" << std::endl;
            }
        }
    }

    // Logica de angajare bazata pe inteligenta
    void obtinePrimulJob() {
        const int inteligenta = stats.getInteligenta().getValoare();

        // Lista de job-uri cu cerinte de inteligenta (sortate de la cel mai slab la cel mai bun)
        const std::vector<Job> joburiDisponibile = {
            // Nume Job (Salariu K, Satisfactie, Cerinta Inteligenta)
            {"Lucrator in Depozit", 20, 30, 0},     // 0-34
            {"Vanzator in Magazin", 35, 40, 35},    // 35-59
            {"Asistent Administrativ", 50, 55, 60}, // 60-79
            {"Programator Junior", 80, 70, 80},     // 80-94
            {"Cercetator Stiintific", 120, 85, 95}  // 95+ (Best Job)
        };

        Job jobAles = joburiDisponibile[0]; // Job de baza (Lucrator in Depozit)

        // Cautare inversa pentru a gasi cel mai bun job pe care personajul il poate obtine
        for (const auto& job : joburiDisponibile) {
            if (inteligenta >= job.cerintaInteligenta) {
                jobAles = job; // Retine cel mai bun job care indeplineste cerinta
            }
        }

        // Aplica noul job personajului
        cariera = Cariera(jobAles.nume, jobAles.salariuAnual, jobAles.satisfactie, jobAles.cerintaInteligenta);

        // Mesaj de notificare (Pop-up)
        std::cout << "\n=================================================" << std::endl;
        std::cout << ">>> FELICITARI! AI IMPLINIT 18 ANI! <<<" << std::endl;
        std::cout << numeComplet << ", datorita inteligentei tale de " << inteligenta
                  << ", ai obtinut primul tau loc de munca:" << std::endl;
        std::cout << ">>> " << cariera.getNumeJob() << " (Salariu Anual: " << cariera.getSalariuAnual() << "K) <<<" << std::endl;
        std::cout << "=================================================" << std::endl;
    }

    // NOU: Intretinere financiara ajustata (Costuri ZERO pana la 18 ani)
    void intretinereFinanciara() {
        double costViata = 0.0; // Implicit: 0 costuri

        // Costuri de viata in K (Mii) - Se aplica doar dupa ce implineste 18 ani
        if (varsta >= 18) {
            costViata = 10.0; // Costuri normale de adult (10K/an)
        }

        // Salvarea banilor DUPA ce am primit salariul in acest an (pentru logica de datorie)
        double baniInitiali = bani;

        // Venitul anual (cariera.getSalariuAnual() este in K)
        bani += cariera.getSalariuAnual();

        // Cheltuieli anuale
        bani -= costViata;

        // Afișează tranzacția financiară anuală
        std::cout << "* FINANCIAR: Salariu primit (+"<< cariera.getSalariuAnual() <<"K), Costuri de trai (-" << costViata << "K)." << std::endl;

        // Verificare datorii si IMPACT
        if (bani < 0 && baniInitiali >= 0.0) { // Intra in datorii (sau adanceste datoria)
            std::cout << "--- IMPACT DATORII ---" << std::endl;
            std::cout << "[! WARNING] AI INTRAT IN DATORII! Fericirea scade cu -10." << std::endl;
            stats.modificaStatistica("Fericire", -10);
            std::cout << "----------------------" << std::endl;
        } else if (bani < 0) {
            std::cout << "[! WARNING] Esti inca in datorii." << std::endl;
        }
    }

    // Functie privata pentru a gestiona scenariile complexe ale expeditiei (Eveniment Major)
    void gestioneazaExpeditiePericuloasa() {
        std::cout << ">> Pregatirea pentru expeditie. Verificam riscurile..." << std::endl;

        // SALVAREA STARII INITIALE
        int sanatateInitiala = stats.getSanatate().getValoare();
        int fericireInitiala = stats.getFericire().getValoare();
        int inteligentaInitiala = stats.getInteligenta().getValoare();
        int aspectInitial = stats.getAspect().getValoare();

        bool pregatit = stats.areStatisticiSanatoase();

        struct Scenariu {
            std::string descriere;
            int impactSanatate;
            int impactFericire;
            int impactInteligenta;
            int impactAspect;
        };

        std::vector<Scenariu> scenarii = {
            // Scenariu 0: Pierdut in Padure (Risc Moderat)
            {"Te-ai ratacit intr-o zona montana. Ai pierdut o zi, dar ai invatat sa te descurci.",
                -5, 0, +15, 0},
            // Scenariu 1: Ajutor la un accident (Succes)
            {"Ai asistat la un accident si ai ajutat la salvarea unei vieti. Esti epuizat, dar fericit.",
                -10, +25, 0, 0},
            // Scenariu 2: Intalnire cu o specie salbatica (Esec)
            {"Ai fost atacat de un animal salbatic. Ai scapat, dar ai suferit rani vizibile.",
                -30, -10, 0, -20},
            // Scenariu 3: Explorare stiintifica (Succes Intelectual)
            {"Ai descoperit o pestera veche cu picturi rupestre. Sanatatea nu este afectata, dar intelectul creste.",
                0, +10, +20, 0},
            // Scenariu 4: Vreme rea si izolare (Risc Minor)
            {"Vremea te-a prins nepregatit. Ai rezistat cu greu. Emotional, esti daramat.",
                0, -15, 0, 0}
        };

        int indexScenariu = getRandomInt(0, static_cast<int>(scenarii.size()) - 1);
        // CORECTIE OPTIMIZARE: Folosim referință constantă
        const Scenariu& s = scenarii[indexScenariu];

        std::cout << "PROMPT: " << s.descriere << std::endl;

        // Aplicam impactul de baza al scenariului
        stats.modificaStatistica("Sanatate", s.impactSanatate);
        stats.modificaStatistica("Fericire", s.impactFericire);
        stats.modificaStatistica("Inteligenta", s.impactInteligenta);
        stats.modificaStatistica("Aspect", s.impactAspect);

        // Modificator de Noroc si Pregatire
        int sansaNoroc = getRandomInt(1, 100);

        if (!pregatit && sansaNoroc < 50) {
            std::cout << "REZULTAT FINAL: Esec critic din lipsa de pregatire! Penalizari suplimentare aplicate." << std::endl;
            stats.modificaStatistica("Sanatate", -20);
            stats.modificaStatistica("Fericire", -15);
        } else if (pregatit && sansaNoroc > 80) {
            std::cout << "REZULTAT FINAL: Echipament excelent si noroc! Ai primit un bonus de fericire." << std::endl;
            stats.modificaStatistica("Fericire", +10);
        } else {
             std::cout << "REZULTAT FINAL: Experienta medie. Impactul de baza a fost aplicat." << std::endl;
        }

        // NOU: AFISAREA IMPACTULUI FINAL CONCIS
        std::cout << "\n--- REZUMAT IMPACT EXPEDITIE ---" << std::endl;

        // Calculeaza si afiseaza diferenta
        auto afiseazaImpact = [&](const std::string& nume, int initial, int final) {
            int diferenta = final - initial;
            if (diferenta != 0) {
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
    // Metoda afiseazaMeniuDecizie()
    static void afiseazaMeniuDecizie() {
        std::cout << "\n--- OPTIUNI ANUALE ---" << std::endl;
        std::cout << "1. Incearca Promovarea/Studiu (Inteligenta)" << std::endl;
        std::cout << "2. Expeditie Periculoasa (Eveniment Major cu Scenarii)" << std::endl;
        std::cout << "3. Creeaza o noua relatie (Prieten/Coleg/Inamic)" << std::endl;
        std::cout << "4. RENUNTA / SURRENDER (Termina jocul)" << std::endl;
        std::cout << "0. Continua la Anul Urmator (Fara Decizie majora)" << std::endl;
    }

    // CONSTRUCTOR MODIFICAT: Primește prin valoare și folosește std::move
    Personaj(const std::string& nume, const std::string& nat, int varsta, const DataNastere& dn, Statistici initialStats)
        : dataNastere(dn), stats(std::move(initialStats)) // CORECTIE CLANG-TIDY APLICATA
    {
        this->numeComplet = nume; this->nationalitate = nat; this->varsta = varsta;
        this->bani = 0.0; // Banii incep de la 0K
        this->varstaDecesAleatorie = getRandomInt(70, 99);
        this->esteMort = false;
        std::cout << "[LOG] Varsta de deces natural setata la " << this->varstaDecesAleatorie << " ani." << std::endl;
    }

    // Constructor Personaj ce foloseste Statistici random (pentru compatibilitate)
    Personaj(const std::string& nume, const std::string& nat, int varsta, const DataNastere& dn)
        : Personaj(nume, nat, varsta, dn, Statistici())
    {}

    [[nodiscard]] const std::string& getNume() const { return numeComplet; }
    [[nodiscard]] int getVarsta() const { return varsta; }

    [[nodiscard]] bool aplicaAnual(int ani = 1) {
        if (esteMort) return true;

        for (int i = 0; i < ani; ++i) {
            if (varsta >= varstaDecesAleatorie || varsta >= VARSTA_MAXIMA_FORTATA) {
                marcheazaDeces("Batranete"); return true;}

            varsta += 1;

            // Obtinerea primului job la 18 ani
            if (varsta == 18) {
                obtinePrimulJob();
            }

            std::cout << "\n=================================================" << std::endl;
            std::cout << "--- START ANUL " << varsta << " | " << numeComplet << " ---" << std::endl;
            std::cout << "=================================================" << std::endl;

            // Progres Anual Automat
            stats.modificaStatistica("Fericire", -VITEZA_DEGRADARE_FERICIRE);
            std::cout << "* Degradare Fericire: -" << VITEZA_DEGRADARE_FERICIRE << std::endl;

            if (verificaDeces()) return true;

            // Logica financiara
            intretinereFinanciara();
            if (verificaDeces()) return true;

            // Logica evenimentelor aleatorii
            evenimentAleatoriu();
            if (verificaDeces()) return true;

            // Logica de Impact Anual al Relatiilor
            std::cout << "\n--- IMPACT ANUAL RELATII ---" << std::endl;
            for (Relatie& r : relatii) {
                // 1. Degradarea anuala de baza (pentru a incuraja interactiunea)
                r.imbunatatesteRelatia(-2);

                int nivelAfectiune = r.getNivelAfectiune();

                // 2. Aplicarea impactului continuu
                if (r.getTipRelatie() == "Prieten" && nivelAfectiune > 70) {
                    stats.modificaStatistica("Fericire", 3);
                    stats.modificaStatistica("Sanatate", 1);
                    std::cout << "* " << r.getNumePersoana() << " (Prieten Afec: " << nivelAfectiune << ") iti aduce bucurie: Fericire +3, Sanatate +1." << std::endl;
                } else if (r.getTipRelatie() == "Inamic" && nivelAfectiune < 30) {
                    stats.modificaStatistica("Fericire", -5);
                    stats.modificaStatistica("Sanatate", -2);
                    std::cout << "* " << r.getNumePersoana() << " (Inamic Afec: " << nivelAfectiune << ") te streseaza: Fericire -5, Sanatate -2." << std::endl;
                } else if (r.getTipRelatie() == "Mama" || r.getTipRelatie() == "Tata") {
                    // Parintii au un efect pozitiv constant, indiferent de afectiune (in limite rezonabile)
                     stats.modificaStatistica("Fericire", 1);
                     std::cout << "* " << r.getNumePersoana() << " (" << r.getTipRelatie() << ") iti ofera un confort constant: Fericire +1." << std::endl;
                }
            }
            std::cout << "----------------------------" << std::endl;
        }
        return esteMort;
    }

    void incepeRelatieNoua(const std::string& nume, const std::string& tip, int afectiune) {
        if (esteMort) { return; }
        Relatie r(nume, tip, afectiune);
        adaugaRelatieIntern(r);
    }

    // Metoda iaDecizieDestin()
    void iaDecizieDestin(int alegere) {
        if (esteMort) return;
        std::cout << "--- EXECUT DECZIA " << alegere << " ---" << std::endl;
        int impactFericire = 0;

        switch (alegere) {
            case 1: { // Promovare/Studiu
                std::cout << "Actiune: Incearca Promovarea. ";
                if (cariera.getNumeJob() != "Somaj") {cariera.incearcaPromovare(stats.getInteligenta().getValoare()); impactFericire += 10;}
                else {std::cout << "Nu ai job. Ai studiat degeaba. "; stats.modificaStatistica("Inteligenta", 5);} break; }
            case 2: { // Expeditie Periculoasa (Eveniment Major)
                std::cout << "Actiune: Initierea unei expeditii periculoase..." << std::endl;
                gestioneazaExpeditiePericuloasa();
                break;
            }
            case 3: { // Creeaza o noua relatie random
                std::cout << "Actiune: Intalnesti pe cineva nou. ";
                creeazaRelatieRandom();
                impactFericire += 15; // Bonus de fericire pentru actiune sociala
                break;}
            case 4: { // Surrender
                std::cout << "Actiune: Jucatorul a renuntat la viata!" << std::endl;
                marcheazaDeces("Renuntare (Surrender)");
                break;
            }
            default: {std::cout << "Alegere invalida. Nicio actiune." << std::endl;}
        }

        if (impactFericire != 0) stats.modificaStatistica("Fericire", impactFericire);

        verificaDeces();
    }

    [[nodiscard]] bool getEsteMort() const {return esteMort;}

    friend std::ostream& operator<<(std::ostream& os, const Personaj& p);
};

// Operatorul de afisare pentru Personaj
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


// ====================================================================
// GRUP 5: FUNCTIA MAIN (CU TESTE DE LOG)
// ====================================================================

int main() {
    std::cout << "--- BITLIFE: DESTINY SIMULATOR v4.0 (FINAL CI FIX) ---" << std::endl;

    // 1. Initializare
    std::string nume, prenume, nationalitate;
    citesteDateIntrare(nume, prenume, nationalitate);

    std::string numeComplet = nume + " " + prenume;
    DataNastere dn = genereazaDataNastere();

    // Generarea statisticilor initiale garantat > 50
    const int MIN_HIGH_STAT = 50;
    const int MAX_HIGH_STAT = 95;

    int s = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);
    int f = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);
    int i = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);
    int a = getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT);

    // LOG: Utilizarea Constructorului cu Parametri al clasei Statistici
    std::cout << "\n[TEST LOG] -----------------------------------------------" << std::endl;
    std::cout << "[TEST LOG] 1. Testam Constructorul Statistici cu Parametri:" << std::endl;
    Statistici stats_initiale(s, f, i, a); // Apel la constructorul cu parametri
    std::cout << "[TEST LOG] Statistici initiale setate: " << stats_initiale << std::endl;
    std::cout << "[TEST LOG] -----------------------------------------------\n" << std::endl;


    std::cout << "\n=================================================" << std::endl;
    std::cout << "Numele personajului: " << numeComplet << std::endl;
    std::cout << "Nationalitatea: " << nationalitate << std::endl;
    std::cout << "Data de nastere: " << dn << std::endl;
    std::cout << "=================================================" << std::endl;

    // 2. Crearea personajului si a relatiilor initiale
    Personaj jucator(numeComplet, nationalitate, 1, dn, stats_initiale); // Transmiterea statisticilor

    std::cout << "\n[TEST LOG] -----------------------------------------------" << std::endl;
    std::cout << "[TEST LOG] 2. Testam Constructorul de Copiere al Relatie (Parinti cu nume specifice):" << std::endl;

    // Mama (Nume din lista de Femei)
    std::string nume_mama = alegeNumeRandom(false);
    jucator.incepeRelatieNoua(nume_mama, "Mama", 100);

    // Tata (Nume din lista de Barbati)
    std::string nume_tata = alegeNumeRandom(true);
    jucator.incepeRelatieNoua(nume_tata, "Tata", 100);
    std::cout << "[TEST LOG] -----------------------------------------------" << std::endl;


    // Test specific pentru Operator= de Copiere
    std::cout << "\n[TEST LOG] 3. Testam Operator= de Copiere al Relatie:" << std::endl;
    Relatie relatie_originala("TestOp", "Prieten", 70);
    Relatie relatie_noua("Inlocuit", "Necunoscut", 10);

    relatie_noua = relatie_originala; // APEL LA OPERATOR= DE COPIERE

    std::cout << "[TEST LOG] Relatia 'relatie_noua' dupa Op=: " << relatie_noua << std::endl;
    std::cout << "[TEST LOG] -----------------------------------------------\n" << std::endl;


    std::cout << "\n--- START VIATA ---" << std::endl;

    // 3. Simulare anuala interactiva
    for (int an_curent = 1; an_curent < VARSTA_MAXIMA_FORTATA; ++an_curent) {
        if (jucator.getEsteMort()) break;

        // Afiseaza starea si meniul de decizie
        std::cout << "\n--- STARE CURENTA ---" << std::endl;
        std::cout << jucator << std::endl;

        // 1. Decizie jucator
        Personaj::afiseazaMeniuDecizie();
        int alegere = citesteAlegere();

        if (alegere != 0) {
            jucator.iaDecizieDestin(alegere);
            if (jucator.getEsteMort()) break;
        }

        // 2. Progres anual automat (aplica schimbarile si avanseaza varsta)
        if (jucator.aplicaAnual(1)) break;
    }

    // 4. Final
    std::cout << "\n=================================================" << std::endl;
    std::cout << "  JOC TERMINAT. REZULTAT FINAL." << std::endl;
    std::cout << "=================================================" << std::endl;

    if (!jucator.getEsteMort()) {
        std::cout << jucator << std::endl;
    }

    // LOG: Testam Destructorul
    std::cout << "\n[TEST LOG] 4. Testam Destructorul Relatie:" << std::endl;
    std::cout << "[TEST LOG] Urmeaza sa iasa din scope Relatiile 'relatie_originala' si 'relatie_noua'..." << std::endl;
    std::cout << "[TEST LOG] Destructorii (inclusiv pentru obiectele Relatie din vectorul jucatorului) vor fi apelati acum (dupa afisarea acestui mesaj)." << std::endl;

    return 0;
}