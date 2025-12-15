#include "Personaj.h"
#include "SimUtilities.h"
#include "AventuraFactory.h"
#include <memory>

void swap(DataNastere& dn1, DataNastere& dn2) noexcept {
    using std::swap;
    swap(dn1.zi, dn2.zi);
    swap(dn1.luna, dn2.luna);
}

int Personaj::nrPersonajeActive = 0;

Personaj::Personaj(const std::string& nume, const std::string& nat, const int v_init, const DataNastere& dn, Statistici initialStats)
: dataNastere(dn), stats(std::move(initialStats))
{
    this->numeComplet = nume;
    this->nationalitate = nat;
    this->varsta = v_init;
    stats.modificaBani(0.0);
    this->varstaDecesAleatorie = GeneratorRandom::getInstance().getRandomInt(70, 99);
    this->esteMort = false;
    nrPersonajeActive++;
    this->initializeazaRelatiiParente();
}

Personaj::Personaj(const std::string& nume, const std::string& nat, const int v_init, const DataNastere& dn)
    : Personaj(nume, nat, v_init, dn, Statistici())
{}

Personaj::Personaj(const Personaj& other) 
    : numeComplet(other.numeComplet), nationalitate(other.nationalitate),
      dataNastere(other.dataNastere), varsta(other.varsta),
      varstaDecesAleatorie(other.varstaDecesAleatorie), esteMort(other.esteMort), 
      stats(other.stats), cariera(other.cariera), relatii(other.relatii)
{
    if (other.ultimaAventura) {
        ultimaAventura.reset(other.ultimaAventura->clone());
    }
    nrPersonajeActive++;
}

Personaj& Personaj::operator=(const Personaj& other) {
    Personaj temp(other); 
    
    using std::swap; 

    swap(this->numeComplet, temp.numeComplet);
    swap(this->nationalitate, temp.nationalitate);
    swap(this->dataNastere, temp.dataNastere);
    swap(this->varsta, temp.varsta);
    swap(this->varstaDecesAleatorie, temp.varstaDecesAleatorie);
    swap(this->esteMort, temp.esteMort);
    swap(this->stats, temp.stats);
    swap(this->cariera, temp.cariera);
    swap(this->relatii, temp.relatii);
    swap(this->ultimaAventura, temp.ultimaAventura);

    return *this;
}

Personaj::~Personaj() {
    nrPersonajeActive--;
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

void Personaj::cumparaProdus(const Shopping& produs) {
    if (esteMort) return;

    const double cost = produs.getCost();

    if (stats.getBani() < cost) {
        std::cout << "[SHOP] Nu ai suficienti bani (" << stats.getBani()
                  << "K) pentru a cumpara " << produs.getNumeProdus()
                  << " (Cost: " << cost << "K)." << std::endl;
        return;
    }

    stats.modificaBani(-cost);
    produs.aplicaImpact(stats);

    adaugaEveniment(varsta, "Cumparatura: " + produs.getNumeProdus(),
                    "Cost: -" + std::to_string(static_cast<int>(cost)) + "K");

    std::cout << "[SHOP] Ai cumparat " << produs.getNumeProdus() << ". Cost: -" << cost << "K." << std::endl;
}

void Personaj::initializeazaRelatiiParente() {
    const std::string numeMama = alegeNumeRandom(false);
    const std::string numeTata = alegeNumeRandom(true);

    const Relatie mama(numeMama, "Mama", 90);
    adaugaRelatieIntern(mama);

    const Relatie tata(numeTata, "Tata", 85);
    adaugaRelatieIntern(tata);

    std::cout << "[INIT] Relatii Parinte adaugate: Mama (" << numeMama << ") si Tata (" << numeTata << ")." << std::endl;
}

void Personaj::creeazaRelatieRandom() {
    if (relatii.size() < MAX_RELATII) {
        const bool eBarbat = (GeneratorRandom::getInstance().getRandomInt(0, 1) == 0);
        const std::string& nume = alegeNumeRandom(eBarbat);
        const std::string& status = STATUS_RELATII[GeneratorRandom::getInstance().getRandomInt(0, static_cast<int>(STATUS_RELATII.size()) - 1)];

        const int aspectScore = stats.getAspect().getValoare();
        int afectiune = GeneratorRandom::getInstance().getRandomInt(20, 95);

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
                afectiune = GeneratorRandom::getInstance().getRandomInt(40, 75);
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

        const std::string impactStr = std::string("F: ")+ (impactFericire > 0 ? "+" : "") + std::to_string(impactFericire)
        + ", S: " + (impactSanatate > 0 ? "+" : "") + std::to_string(impactSanatate);

        adaugaEveniment(varsta, "Noua relatie: " + status + " cu " + nume, impactStr);

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
    if (relatii.empty()) return;

    const int fericireInitiala = stats.getFericire().getValoare();
    const double baniInitiali = stats.getBani();

    if (const int sansa = GeneratorRandom::getInstance().getRandomInt(1, 100); sansa < 35) {

        const int impact = GeneratorRandom::getInstance().getRandomInt(-15, 20);

        std::string descriere = "S-a intamplat un eveniment minor in viata ta.";
        if (impact < 0) {
            descriere = "Ai pierdut un obiect valoros.";
            stats.modificaStatistica("Fericire", impact);
        }
        else
            if (impact > 10) {
                descriere = "Ai gasit 50 de lei pe strada!";
                stats.modificaBani(0.05);
                stats.modificaStatistica("Fericire", impact);
            }
            else {
                stats.modificaStatistica("Fericire", impact);
            }

        std::cout << "* Eveniment Aleatoriu: " << descriere << std::endl;

        auto& relatie_afectata = const_cast<Relatie&>(alegeElementAleatoriu(relatii));

        if (GeneratorRandom::getInstance().getRandomInt(1, 100) < 50) {
            const int schimbare_afectiune = GeneratorRandom::getInstance().getRandomInt(-5, 5);
            relatie_afectata.imbunatatesteRelatia(schimbare_afectiune);

            std::cout << "  - Relatia cu " << relatie_afectata.getNumePersoana()
                      << " este afectata: " << (schimbare_afectiune > 0 ? "+" : "")
                      << schimbare_afectiune << " Afectiune." << std::endl;
        }

        if (GeneratorRandom::getInstance().getRandomInt(1, 10) > 8) {
            creeazaRelatieRandom();
        }

        const int fericireFinala = stats.getFericire().getValoare();
        const double baniFinali = stats.getBani();

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

    const double baniInitiali = stats.getBani();
    stats.modificaBani(cariera.getSalariuAnual());
    stats.modificaBani(-costViata);

    std::cout << "* FINANCIAR: Salariu primit (+"<< cariera.getSalariuAnual() <<"K), Costuri de trai (-" << costViata << "K)." << std::endl;

    if (stats.getBani() < 0 && baniInitiali >= 0.0) {
        std::cout << "--- IMPACT DATORII ---" << std::endl;
        stats.modificaStatistica("Fericire", -10);
        std::cout << "----------------------" << std::endl;
    }
    else
        if (stats.getBani() < 0) {
        std::cout << "[! ATENTIE] Esti inca in datorii." << std::endl;
        }
}
void Personaj::sePreparaPentruAventura() {
    constexpr int COST_FERICIRE = 10;

    if (this->varsta < 10) {
        std::cout << "[PREPARARE] Prea mic pentru antrenamente serioase. Jucat în parc! Fericire +5." << std::endl;
        stats.modificaStatistica("Fericire", 5);
        return;
    }

    stats.modificaStatistica("Fericire", -COST_FERICIRE);

    const int bonusSanatate = stats.getSanatate().getValoare() / 15;
    const int bonusInteligenta = stats.getInteligenta().getValoare() / 20;

    int bonusObtinut = 10 + bonusSanatate + bonusInteligenta;

    if (bonusObtinut > 30) {
        bonusObtinut = 30;
    }

    this->bonusAventuraTemporar = bonusObtinut;

    std::cout << "\n[PREPARARE INTENSIVA] Te-ai pregatit pentru riscuri. Cost Fericire: -" << COST_FERICIRE << ". ";
    std::cout << "Bonus de succes (+%) obtinut pentru urmatoarea aventura: +" << this->bonusAventuraTemporar << "%." << std::endl;
}

void Personaj::reseteazaBonusAventura() {
    this->bonusAventuraTemporar = 0;
}
void Personaj::gestioneazaExpeditiePericuloasa() {
    std::cout << ">> Selectarea unei aventuri pentru anul curent..." << std::endl;

    const bool pregatit = stats.areStatisticiSanatoase();

    ultimaAventura = AventuraFactory::creeazaAventuraAleatoare();
    
    std::cout << ">> Tip Aventura: ";
    ultimaAventura->afiseazaDetalii(std::cout); 
    std::cout << std::endl;

    const int riscAventura = ultimaAventura->getSansaEsecBaza();
    const int riscFinal = std::max(10, riscAventura - this->bonusAventuraTemporar);

    std::cout << ">> Risc Baza: " << riscAventura << "%. Bonus Pregatire: -" << this->bonusAventuraTemporar << "%. Risc Final (Simulat): " << riscFinal << "%." << std::endl;

        ultimaAventura->aplicaImpact(stats);

    this->reseteazaBonusAventura();
        
        if (dynamic_cast<Salvare*>(ultimaAventura.get())) {
            Salvare::oferaMotivatieExtra(stats);
            std::cout << "\n[LOG: Downcast] Primit motivatie extra (Fericire +5) de la Salvare.";
        }

        if (!pregatit && GeneratorRandom::getInstance().getRandomInt(1, 100) < 50) {
             std::cout << "\n[ATENTIE] Nepregatirea a condus la o penalizare critica!";
             stats.modificaStatistica("Sanatate", -15);
             stats.modificaStatistica("Fericire", -15);
        } else if (pregatit && GeneratorRandom::getInstance().getRandomInt(1, 100) > 80) {
             std::cout << "\n[BONUS] Pregatirea a asigurat un mic bonus!";
             stats.modificaStatistica("Fericire", 5);
        }

    std::cout << "\n--------------------------------\n" << std::endl;
}

void Personaj::evenimentViataMajor() {
    if (esteMort || !numeSotSauSotie.empty() || varsta < 23 || varsta > 40) {
        return;
    }

    if (GeneratorRandom::getInstance().getRandomInt(1, 100) > 70) {
        return;
    }

    for (const auto& r : relatii) {
        if (r.getNivelAfectiune() >= 65 && (r.getTipRelatie() == "Prieten" || r.getTipRelatie() == "Coleg")) {

            constexpr double COST_NUNTA = 50.0;
            constexpr int BONUS_FERICIRE = 30;

            numeSotSauSotie = r.getNumePersoana();
            stats.modificaBani(-COST_NUNTA);
            stats.modificaStatistica("Fericire", BONUS_FERICIRE);

            const std::string descriereNunta = "Casatorie cu " + numeSotSauSotie;
            const std::string impactNunta = std::string("Fericire +")
                                    + std::to_string(BONUS_FERICIRE)
                                    + ", Cost: -"
                                    + std::to_string(static_cast<int>(COST_NUNTA)) + "K";

            adaugaEveniment(varsta, descriereNunta, impactNunta);

            std::cout << "\n[EVENIMENT MAJOR] NUNTA la " << varsta << " de ani cu " << numeSotSauSotie << "!" << std::endl;

            return;
        }
    }
}

void Personaj::afiseazaMeniuDecizie() {
    std::cout << "\n--- OPTIUNI ANUALE PRINCIPALE ---" << std::endl;
    std::cout << "0. Continua la Anul Urmator (Fara Decizie majora)" << std::endl;
    std::cout << "1. Incearca Promovarea/Studiu (Inteligenta)" << std::endl;
    std::cout << "2. Expeditie Periculoasa (Eveniment Major cu Scenarii)" << std::endl;
    std::cout << "3. Creeaza o noua relatie (Prieten/Coleg/Inamic)" << std::endl;
    std::cout << "4. PREPARARE: Antrenament pentru reducerea riscului (Cost Fericire)" << std::endl;
    std::cout << "\n--- OPTIUNI SHOPPING ---" << std::endl;
    std::cout << "5. Cumpara Casa (250K) ++++ FERICIRE ++++" << std::endl;
    std::cout << "6. Sesiune Spa (5K) ++++ FERICIRE & ASPECT ++++" << std::endl;
    std::cout << "7. Cumpara Carti (0.5K) ++++ INTELIGENTA ++++" << std::endl;
    std::cout << "8. Cumpara Masina (50K) ++++ FERICIRE ++++" << std::endl;
    std::cout << "9. Sesiune Medicamente (0.1K) ++++ SANATATE ++++" << std::endl;
    std::cout << "\n--- FINALIZARE JOC ---" << std::endl;
    std::cout << "10. RENUNTA / SURRENDER" << std::endl;
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
            std::cout << "Actiune: Initierea pregatirii anuale..." << std::endl;
            sePreparaPentruAventura();
            break;
        }
        case 5: {
            std::cout << "Actiune: Cumpararea unei case..." << std::endl;
            const CumparaCasa casa;
            cumparaProdus(casa);
            break;
        }
        case 6: {
            std::cout << "Actiune: Cumpararea unei sesiuni la spa..." << std::endl;
            const SesiuneSpa spa;
            cumparaProdus(spa);
            break;
        }
        case 7: {
            std::cout << "Actiune: Cumpararea unor carti..." << std::endl;
            const CumparaCarti carti;
            cumparaProdus(carti);
            break;
        }
        case 8: {
            std::cout << "Actiune: Cumpararea unei masini..." << std::endl;
            const CumparaMasina masina;
            cumparaProdus(masina);
            break;
        }
        case 9: {
            std::cout << "Actiune: Cumpararea unor medicamente..." << std::endl;
            const CumparaCasa casa;
            cumparaProdus(casa);
            break;
        }
        case 10: {
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
        evenimentViataMajor();
        if (verificaDeces()) return true;

        std::cout << "\n--- IMPACT ANUAL RELATII ---" << std::endl;
        for (Relatie& r : relatii) {
            r.imbunatatesteRelatia(-2);

            if (const int nivelAfectiune = r.getNivelAfectiune(); r.getTipRelatie() == "Prieten" && nivelAfectiune > 50) {
                stats.modificaStatistica("Fericire", 3);
                stats.modificaStatistica("Sanatate", 1);
                std::cout << "* " << r.getNumePersoana() << " (Prieten Afec: " << nivelAfectiune << ") iti aduce bucurie: Fericire +3, Sanatate +1." << std::endl;
            }
            else
                if (r.getTipRelatie() == "Inamic" && nivelAfectiune < 30) {
                    stats.modificaStatistica("Fericire", -4);
                    stats.modificaStatistica("Sanatate", -4);
                    std::cout << "* " << r.getNumePersoana() << " (Inamic Afec: " << nivelAfectiune << ") te streseaza: Fericire -4, Sanatate -4." << std::endl;
                }
                else
                    if (r.getTipRelatie() == "Mama" || r.getTipRelatie() == "Tata") {
                        stats.modificaStatistica("Fericire", 5);
                        std::cout << "* " << r.getNumePersoana() << " (" << r.getTipRelatie() << ") iti ofera un confort constant: Fericire +5." << std::endl;
                    }
        }
        std::cout << "----------------------------" << std::endl;
    }
    return esteMort;
}

void Personaj::adaugaEveniment(int v_init, const std::string& descriere, const std::string& impact) {
    istoricEvenimente.emplace_back(v_init, descriere, impact);
}

void Personaj::afiseazaIstoricViata() const {
    std::cout << "\n=================================================" << std::endl;
    std::cout << ">>> ISTORIC CRONOLOGIC: " << numeComplet << " <<<" << std::endl;
    std::cout << "=================================================" << std::endl;

    for (const auto& ev : istoricEvenimente) {
        std::cout << ev << std::endl;
    }
    std::cout << "=================================================" << std::endl;
}


std::ostream& operator<<(std::ostream& os, const Personaj& p) {
    os << "--- " << p.numeComplet << " (" << p.varsta << " ani, " << p.nationalitate << ") ---" << "\n";
    os << " Data Nastere: " << p.dataNastere << "\n";
    os << " BANI: " << std::fixed << std::setprecision(2) << p.stats.getBani() << "K | ";
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