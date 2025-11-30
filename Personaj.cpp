#include "Exceptii.h"
#include "Personaj.h"

void swap(DataNastere& dn1, DataNastere& dn2) noexcept {
    using std::swap;
    swap(dn1.zi, dn2.zi);
    swap(dn1.luna, dn2.luna);
}

int Personaj::nrPersonajeActive = 0;

Personaj::Personaj(const std::string& nume, const std::string& nat, const int varsta, const DataNastere& dn, Statistici initialStats)
: dataNastere(dn), stats(std::move(initialStats))
{
    this->numeComplet = nume;
    this->nationalitate = nat;
    this->varsta = varsta;
    this->bani = 0.0;
    this->varstaDecesAleatorie = getRandomInt(70, 99);
    this->esteMort = false;
    Personaj::nrPersonajeActive++;
}

Personaj::Personaj(const std::string& nume, const std::string& nat, const int varsta, const DataNastere& dn)
    : Personaj(nume, nat, varsta, dn, Statistici())
{}

Personaj::Personaj(const Personaj& other) 
    : numeComplet(other.numeComplet), nationalitate(other.nationalitate), 
      dataNastere(other.dataNastere), varsta(other.varsta), bani(other.bani), 
      varstaDecesAleatorie(other.varstaDecesAleatorie), esteMort(other.esteMort), 
      stats(other.stats), cariera(other.cariera), relatii(other.relatii)
{
    if (other.ultimaAventura) {
        ultimaAventura.reset(other.ultimaAventura->clone());
    }
    Personaj::nrPersonajeActive++;
}

Personaj& Personaj::operator=(const Personaj& other) {
    Personaj temp(other); 
    
    using std::swap; 

    swap(this->numeComplet, temp.numeComplet);
    swap(this->nationalitate, temp.nationalitate);
    swap(this->dataNastere, temp.dataNastere);
    swap(this->varsta, temp.varsta);
    swap(this->bani, temp.bani);
    swap(this->varstaDecesAleatorie, temp.varstaDecesAleatorie);
    swap(this->esteMort, temp.esteMort);
    swap(this->stats, temp.stats);
    swap(this->cariera, temp.cariera);
    swap(this->relatii, temp.relatii);
    swap(this->ultimaAventura, temp.ultimaAventura);

    return *this;
}

Personaj::~Personaj() {
    Personaj::nrPersonajeActive--;
}

int Personaj::getNrPersonajeActive() { return nrPersonajeActive; }
[[nodiscard]] int Personaj::getVarsta() const { return varsta; }
[[nodiscard]] bool Personaj::getEsteMort() const {return esteMort;}

void Personaj::adaugaRelatieIntern(const Relatie& r) {
    if (relatii.size() < MAX_RELATII) {
        relatii.push_back(r);
    }
    else {
        std::cout << "Limita de relatii atinsa." << std::endl;
    }
}

void Personaj::creeazaRelatieRandom() {
    if (relatii.size() < MAX_RELATII) {
        const bool eBarbat = (getRandomInt(0, 1) == 0);
        const std::string& nume = alegeNumeRandom(eBarbat);
        const std::string& status = STATUS_RELATII[getRandomInt(0, static_cast<int>(STATUS_RELATII.size()) - 1)];

        const int aspectScore = stats.getAspect().getValoare();
        int afectiune = getRandomInt(20, 95);

        afectiune = std::max(20, std::min(95, afectiune + (aspectScore / 4)));

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
                afectiune = std::min(75, afectiune + (aspectScore / 6));
            }
            else
                if (status == "Inamic") {
                    impactFericire = -15;
                    impactSanatate = -5;
                    if (aspectScore > 80)
                        afectiune = std::min(10, afectiune);
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

void Personaj::marcheazaDeces(const std::string& cauza) {
    if (esteMort) { return; }
    esteMort = true;
    std::cout << "=================================================" << std::endl;
    std::cout << "=== JOC TERMINAT! Jucatorul a murit la " << varsta << " ani. ===" << std::endl;
    std::cout << "=== Cauza: " << cauza << " ===" << std::endl;
    std::cout << "=================================================" << std::endl;
}

bool Personaj::verificaDeces() {
    if (esteMort) return true;
    
    if (stats.getSanatate().getValoare() < PRAG_SANATATE_CRITICA) {
        marcheazaDeces("Sanatate extrem de scazuta");
        return true;
    }
    return false;
}

void Personaj::evenimentAleatoriu() {
    const int fericireInitiala = stats.getFericire().getValoare();
    const double baniInitiali = bani;

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

void Personaj::obtinePrimulJob() {
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

void Personaj::intretinereFinanciara() {
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
        stats.modificaStatistica("Fericire", -10);
        std::cout << "----------------------" << std::endl;
    }
    else
        if (bani < 0) {
        std::cout << "[! ATENTIE] Esti inca in datorii." << std::endl;
        }
}

void Personaj::gestioneazaExpeditiePericuloasa() {
    std::cout << ">> Selectarea unei aventuri pentru anul curent..." << std::endl;

    const int tip = getRandomInt(1, 3); 
    const bool pregatit = stats.areStatisticiSanatoase(); 

    if (tip == 1) {
        ultimaAventura.reset(new DrumetieMontana());
    } else if (tip == 2) {
        ultimaAventura.reset(new Salvare());
    } else { 
        ultimaAventura.reset(new Vanatoare());
    }
    
    std::cout << ">> Tip Aventura: ";
    ultimaAventura->afiseazaDetalii(std::cout); 
    std::cout << std::endl;

    try {
        ultimaAventura->aplicaImpact(stats); 
        
        if (auto* salvarePtr = dynamic_cast<Salvare*>(ultimaAventura.get())) {
            salvarePtr->oferaMotivatieExtra(stats); 
            std::cout << "\n[LOG: Downcast] Primit motivatie extra (Fericire +5) de la Salvare.";
        }

        if (!pregatit && getRandomInt(1, 100) < 50) {
             std::cout << "\n[ATENTIE] Nepregatirea a condus la o penalizare critica!";
             stats.modificaStatistica("Sanatate", -15);
             stats.modificaStatistica("Fericire", -15);
        } else if (pregatit && getRandomInt(1, 100) > 80) {
             std::cout << "\n[BONUS] Pregatirea a asigurat un mic bonus!";
             stats.modificaStatistica("Fericire", 5);
        }

    }
    catch (const EroareStatisticaCritica& e) {
        std::cout << "\n[ACTIUNE FATALA] " << e.what() << std::endl;
        marcheazaDeces("Efect Fatal in timpul Expeditiei");
        ultimaAventura.reset(nullptr); 
        return;
    }

    std::cout << "\n--------------------------------\n" << std::endl;
}

void Personaj::afiseazaMeniuDecizie() {
    std::cout << "\n--- OPTIUNI ANUALE (CITITE DIN FISIER) ---" << std::endl;
    std::cout << "0. Continua la Anul Urmator (Fara Decizie majora)" << std::endl;
    std::cout << "1. Incearca Promovarea/Studiu (Inteligenta)" << std::endl;
    std::cout << "2. Expeditie Periculoasa (Eveniment Major cu Scenarii)" << std::endl;
    std::cout << "3. Creeaza o noua relatie (Prieten/Coleg/Inamic)" << std::endl;
    std::cout << "4. RENUNTA / SURRENDER (Termina jocul)" << std::endl;
}

void Personaj::incepeRelatieNoua(const std::string& nume, const std::string& tip, const int afectiune) {
    if (esteMort) { return; }
    const Relatie r(nume, tip, afectiune);
    adaugaRelatieIntern(r);
}

void Personaj::iaDecizieDestin(const int alegere) {
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

[[nodiscard]] bool Personaj::aplicaAnual(const int ani) {
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