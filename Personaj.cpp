#include "Personaj.h"
#include "SimUtilities.h"
#include "AventuraFactory.h"
#include "Sentinta.h"
#include "Vacanta.h"
#include "Statistici.h"
#include "Exceptii.h"
#include "Crima.h"
#include <memory>
#include <cmath>

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
    this->aniSentintaRamasi = 0;
    this->cazierNivel = 0;
    this->bonusAventuraTemporar = 0;
    this->pensiaAnuala = 0.0;
    this->estePensionar = false;
    this->numarCopii = 0;
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
      stats(other.stats), cariera(other.cariera), relatii(other.relatii),
      aniSentintaRamasi(other.aniSentintaRamasi), cazierNivel(other.cazierNivel),
      bonusAventuraTemporar(other.bonusAventuraTemporar), numeSotSauSotie(other.numeSotSauSotie)
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
    swap(this->aniSentintaRamasi, temp.aniSentintaRamasi);
    swap(this->cazierNivel, temp.cazierNivel);
    swap(this->bonusAventuraTemporar, temp.bonusAventuraTemporar);
    swap(this->numeSotSauSotie, temp.numeSotSauSotie);

    return *this;
}

Personaj::~Personaj() {
    nrPersonajeActive--;
}

int Personaj::getNrPersonajeActive() { return nrPersonajeActive; }
[[nodiscard]] int Personaj::getVarsta() const { return varsta; }
[[nodiscard]] bool Personaj::getEsteMort() const {return esteMort;}
[[nodiscard]] const std::string& Personaj::getNumeComplet() const {return numeComplet;}

void Personaj::setEsteMort(const bool status) {
    if (this->esteMort == status) return;

    this->esteMort = status;

    if (status) {
        marcheazaDeces("Cauza externa (Boala Cronica sau Incident)");
    }
}

void Personaj::verificaDecesParinti() {
    for (size_t i = 0; i < relatii.size(); ++i) {
        Relatie& r = relatii[i];

        if (r.getTipRelatie() != "Mama" && r.getTipRelatie() != "Tata") {
            continue;
        }
        const int nouaVarsta = r.getVarsta() + 1;
        r.setVarsta(nouaVarsta);

        if (constexpr int VARSTA_RISC_MAX = 70; r.getVarsta() >= VARSTA_RISC_MAX) {

            double riscDeces = 0.005;
            riscDeces += (r.getVarsta() - VARSTA_RISC_MAX) * 0.02;

            if (GeneratorRandom::getInstance().getDouble(0.0, 1.0) < riscDeces) {

                std::cout << "\n*** TRAGEDIE FAMILIALA! ***" << std::endl;
                std::cout << r.getTipRelatie() << " " << r.getNumePersoana() << " a decedat la varsta de "
                          << r.getVarsta() << " ani." << std::endl;

                modificaStatistica("Fericire", -40);

                adaugaEveniment(varsta, "Deces Parinte", r.getTipRelatie() + " " + r.getNumePersoana() + " a murit. Fericire -40.");

                relatii.erase(relatii.begin() + static_cast<long long>(i));
                i--;

                if (verificaDeces()) return;
            }
        }
    }
}

void Personaj::adaugaRelatieIntern(const Relatie& r) {
    if (relatii.size() < MAX_RELATII) {
        relatii.push_back(r);
    }
    else {
        std::cout << "Limita de relatii atinsa." << std::endl;
    }
}

const Statistici &Personaj::getStatistici() const {
    return stats;
}

void Personaj::modificaStatistica(const std::string& nume, const double valoare) {
    if (nume == "Bani") {
        stats.modificaBani(valoare);
    }
    else if (nume == "Fericire" || nume == "Sanatate" || nume == "Inteligenta" || nume == "Aspect" || nume == "Vizibilitate") {
        stats.modificaStatistica(nume, static_cast<int>(std::round(valoare)));
    }
    else {
        std::cerr << "[EROARE LOGICA] Statistica necunoscuta: " << nume << std::endl;
    }
}

void Personaj::planificaVacanta() {
    if (esteMort) return;
    const Vacanta vacantaNoua;
    std::cout << "\n[DECIZIE VACANTA] Esti gata sa mergi in vacanta la "
              << vacantaNoua.getDestinatie().nume << " (Cost: "
              << vacantaNoua.getDestinatie().costBaza << "K)?" << std::endl;

    vacantaNoua.organizeaza(stats, relatii);
    const std::string impactVacanta = std::string("Fericire +")
                                    + std::to_string(vacantaNoua.getDestinatie().bonusFericire)
                                    + ", Cost: -"
                                    + std::to_string(vacantaNoua.getDestinatie().costBaza) + "K";
    adaugaEveniment(varsta, "Vacanta la " + vacantaNoua.getDestinatie().nume, impactVacanta);
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

void Personaj::pensioneaza() {
    if (estePensionar) return;

    const double SALARIU_FINAL = cariera.getSalariuAnual();
    constexpr double RATA_PENSIE = 0.60;

    pensiaAnuala = SALARIU_FINAL * RATA_PENSIE;
    estePensionar = true;

    stats.modificaStatistica("Fericire", 15);

    std::cout << "\n=================================================" << std::endl;
    std::cout << ">>> PENSIONARE! <<<" << std::endl;
    std::cout << numeComplet << " s-a pensionat la varsta de " << varsta << " ani." << std::endl;
    std::cout << "Pensia Anuală calculata (60% din salariul final): " << std::fixed
              << std::setprecision(2) << pensiaAnuala << "K." << std::endl;
    std::cout << "=================================================" << std::endl;

    adaugaEveniment(varsta, "Pensionare (Final Cariera)", "Venit redus, Fericire +15.");
    notifica("PENSIONARE");
}

void Personaj::actiuneSocialMedia() {
    if (esteMort) return;
    retea.posteazaCeva(stats, relatii);

    adaugaEveniment(varsta, "Postare Social Media", "Vizibilitate/Fericire/Relatii modificate.");
}

void Personaj::initializeazaRelatiiParente() {
    const std::string numeMama = alegeNumeRandom(false);
    const std::string numeTata = alegeNumeRandom(true);

    const Relatie mama(numeMama, "Mama", GeneratorRandom::getInstance().getRandomInt(75, 100), GeneratorRandom::getInstance().getRandomInt(18, 40));
    adaugaRelatieIntern(mama);

    const Relatie tata(numeTata, "Tata", GeneratorRandom::getInstance().getRandomInt(75, 100), GeneratorRandom::getInstance().getRandomInt(18, 40));
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

        const Relatie r(nume, status, afectiune, varsta);
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
    double venitAnual = 0.0;
    if (varsta >= 18) {
        costViata = 10.0;
    }
    costViata += (numarCopii * 3.0);

    const double baniInitiali = stats.getBani();
    stats.modificaBani(cariera.getSalariuAnual());
    stats.modificaBani(-costViata);

    if (estePensionar) {
        venitAnual = pensiaAnuala;
        std::cout << "* FINANCIAR: Pensie primita (+"<< std::fixed << std::setprecision(2) << venitAnual <<"K), ";
    } else {
        venitAnual = cariera.getSalariuAnual();
        std::cout << "* FINANCIAR: Salariu primit (+"<< std::fixed << std::setprecision(2) << venitAnual <<"K), ";
    }
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
            notifica("CASATORIE");
            std::cout << "\n[EVENIMENT MAJOR] NUNTA la " << varsta << " de ani cu " << numeSotSauSotie << "!" << std::endl;

            return;
        }
    }
}

void Personaj::aplicaSentinta(const Sentinta& s) {
    if (stats.getBani() >= s.getAmendaValoare()) {
        modificaStatistica("Bani", -s.getAmendaValoare());
        std::cout << "  [AMENDA] " << s.getAmendaValoare() << " K au fost platiti.\n";
    } else {
        const int aniSuplimentari = static_cast<int>(std::round(s.getAmendaValoare() / 100000.0));
        aniSentintaRamasi = s.getDurataInchisoare() + aniSuplimentari;
        std::cout << "  [FALIMENT] Nu ai avut bani pentru amenda. Sentinta extinsa cu " << aniSuplimentari << " ani.\n";
    }

    if (aniSentintaRamasi == 0) {
        aniSentintaRamasi = s.getDurataInchisoare();
    }

    cazierNivel += s.getImpactCazierFinal();
    if (cazierNivel > 100) cazierNivel = 100;
    std::cout << "  [CAZIER] Nivel cazier actualizat la " << cazierNivel << ".\n";

    modificaStatistica("Fericire", -30);
    modificaStatistica("Sanatate", -10);

    std::cout << "\n>>> VERDICT FINAL: ";
    if (aniSentintaRamasi > 0) {
        std::cout << "CONDAMNARE! SENTINTA DE " << aniSentintaRamasi << " ANI INCHISOARE." << std::endl;
    } else {
        std::cout << "Achitare (doar amenda/fara inchisoare)." << std::endl;
    }
    std::cout << "------------------------------------------\n";
}

bool Personaj::esteInInchisoare() const {
    return aniSentintaRamasi > 0;
}

void Personaj::ruleazaAnInInchisoare() {
    aniSentintaRamasi--;
    varsta++;

    std::cout << "\n==================================================\n";
    std::cout << "    Anul " << varsta << " - INCHISOARE. Ani ramasi: " << aniSentintaRamasi << "\n";
    std::cout << "==================================================\n";

    modificaStatistica("Fericire", -15.0);
    modificaStatistica("Sanatate", -10.0);

    if (aniSentintaRamasi == 0) {
        std::cout << "[ELIBERARE] Felicitari! Ai fost eliberat. Cauta un nou job.\n";
        modificaStatistica("Fericire", 30.0);
        modificaStatistica("Sanatate", 10.0);
    }
}

void Personaj::executaActiuneCrima(const int crimaID) {
    if (esteInInchisoare()) {
        std::cout << "[INFO] Nu se poate comite crima in inchisoare. Actiune ignorata.\n";
        return;
    }

    // Verifică și aruncă excepție dacă ID-ul este invalid
    if (const Crima* crima = Crima::getCrimaPeDecizie(crimaID)) {
        std::cout << "Executa Crima: " << crima->getNume() << "..." << std::endl;
        crima->executa(*this);
    } else {
        throw EroareDecizieInvalida(crimaID);
    }
}


void Personaj::afiseazaMeniuDecizie() {
    std::cout << "\n--- OPTIUNI ANUALE PRINCIPALE ---" << std::endl;
    std::cout << "0. Continua la Anul Urmator (Fara Decizie majora)" << std::endl;
    std::cout << "1. Incearca Promovarea/Studiu (Inteligenta)" << std::endl;
    std::cout << "2. Expeditie Periculoasa (Eveniment Major cu Scenarii)" << std::endl;
    std::cout << "3. Creeaza o noua relatie (Prieten/Coleg/Inamic)" << std::endl;
    std::cout << "4. PREPARARE: Antrenament pentru reducerea riscului (Cost Fericire)" << std::endl;
    std::cout << "5. Planifica Vacanta (Destinatie si Parteneri aleatorii)" << std::endl;
    std::cout << "6: Actiune Retele Sociale" << std::endl;
    std::cout << "\n--- OPTIUNI SHOPPING ---" << std::endl;
    std::cout << "7. Cumpara Casa (250K) ++++ FERICIRE ++++" << std::endl;
    std::cout << "8. Sesiune Spa (5K) ++++ FERICIRE & ASPECT ++++" << std::endl;
    std::cout << "9. Cumpara Carti (0.5K) ++++ INTELIGENTA ++++" << std::endl;
    std::cout << "10. Cumpara Masina (50K) ++++ FERICIRE ++++" << std::endl;
    std::cout << "11. Cumpara Medicamente (0.1K) ++++ SANATATE ++++" << std::endl;
    std::cout << "\n--- OPTIUNI ILEGALE (CRIME) ---" << std::endl;
    std::cout << "12. Comite CRIMA SIMPLA (Ex: Furt, Vandalism)" << std::endl;
    std::cout << "13. Comite CRIMA GRAVA (Ex: Jaf, Vanzare Ilegala)" << std::endl;
    std::cout << "\n--- FINALIZARE JOC ---" << std::endl;
    std::cout << "14. RENUNTA / SURRENDER" << std::endl;
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
            std::cout << "Actiune: Planificare Vacanta." << std::endl;
            planificaVacanta();
            break;
        }
        case 6: {
            std::cout << "Initiere Postare Social Media." << std::endl;
            actiuneSocialMedia();
            break;
        }
        case 7: {
            std::cout << "Actiune: Cumpararea unei case..." << std::endl;
            const CumparaCasa casa;
            cumparaProdus(casa);
            break;
        }
        case 8: {
            std::cout << "Actiune: Cumpararea unei sesiuni la spa..." << std::endl;
            const SesiuneSpa spa;
            cumparaProdus(spa);
            break;
        }
        case 9: {
            std::cout << "Actiune: Cumpararea unor carti..." << std::endl;
            const CumparaCarti carti;
            cumparaProdus(carti);
            break;
        }
        case 10: {
            std::cout << "Actiune: Cumpararea unei masini..." << std::endl;
            const CumparaMasina masina;
            cumparaProdus(masina);
            break;
        }
        case 11: {
            std::cout << "Actiune: Cumpararea unor medicamente..." << std::endl;
            const CumparaMedicamente medicamente;
            cumparaProdus(medicamente);
            break;
        }
        case 12: {
            std::cout << "Actiune: Comitere CRIMA SIMPLA. Se alege tipul..." << std::endl;
            const int crimaID = GeneratorRandom::getInstance().getRandomInt(20, 22);
            executaActiuneCrima(crimaID);
            break;
        }
        case 13: {
            std::cout << "Actiune: Comitere CRIMA GRAVA. Se alege tipul..." << std::endl;
            const int crimaID = GeneratorRandom::getInstance().getRandomInt(23, 25);
            executaActiuneCrima(crimaID);
            break;
        }
        case 14: {
            std::cout << "Actiune: Jucatorul a renuntat la viata!" << std::endl;
            marcheazaDeces("Renuntare (Surrender)");
            break;
        }
        case 0: {
            std::cout << "Actiune: Nicio decizie majora luata. Se trece la anul urmator." << std::endl;
            break;
        }
        default: {
            throw EroareDecizieInvalida(alegere);
        }
    }

    if (impactFericire != 0)
        stats.modificaStatistica("Fericire", impactFericire);

    verificaDeces();
}

[[nodiscard]] bool Personaj::aplicaAnual(const int ani) {
    if (esteMort) return true;

    for (int i = 0; i < ani; ++i) {
        if (esteInInchisoare()) {
            ruleazaAnInInchisoare();
            continue;
        }
        if (varsta == 65 && !estePensionar) {
            pensioneaza();
        }

        if (varsta >= 18 && varsta <= 40 && !numeSotSauSotie.empty()) {
            if (GeneratorRandom::getInstance().getRandomInt(1, 100) <= 5) {
                numarCopii++;
                if (numarCopii == 1) notifica("PRIMUL_COPIL");
                std::cout << "\n+++ EVENIMENT MAJOR! +++" << std::endl;
                std::cout << "Ai un copil! Acum ai " << numarCopii << " copii." << std::endl;
                std::cout << "+++ COST INITIAL: Bani -5K, Fericire -5. +++\n";

                modificaStatistica("Fericire", -5);
                modificaStatistica("Bani", -5.0);

                adaugaEveniment(varsta, "Nasterea unui Copil", "A aparut un membru nou in familie.");
            }
        }

        for (const auto& boala : boliCronice) {
            boala.aplicaEfectAnual(*this);
            if (verificaDeces())
                return true;
        }

        if (varsta >= 18) {
            double riscAnualBoala = 0.005 + static_cast<double>(varsta - 18) * 0.0005;

            if (const double sanatateActuala = stats.getStatistica("Sanatate"); sanatateActuala < 50) {
                riscAnualBoala *= 2.0;
            }

            if (GeneratorRandom::getInstance().getDouble(0.0, 1.0) < riscAnualBoala) {

                std::vector<Boala> boliPosibile = {
                    Boala("Diabet Tip II", -1.0, 3.0, 0.001),
                    Boala("Hipertensiune Arteriala", -0.8, 2.5, 0.002),
                    Boala("Boli de Inima Cronice", -2.5, 10.0, 0.015),
                    Boala("Astm Cronic", -0.5, 1.5, 0.0005)
                };
                const int indexMaxim = static_cast<int>(boliPosibile.size() - 1);
                const int idxBoala = GeneratorRandom::getInstance().getRandomInt(0, indexMaxim);

                bool dejaExista = false;
                for (const auto& existingBoala : boliCronice) {
                    if (existingBoala.getNume() == boliPosibile[idxBoala].getNume()) {
                        dejaExista = true;
                        break;
                    }
                }

                if (!dejaExista) {
                    boliCronice.push_back(boliPosibile[idxBoala]);

                    std::cout << "\n!!! AVERTISMENT SANATATE !!!" << std::endl;
                    std::cout << numeComplet << " a contractat " << boliPosibile[idxBoala].getNume() << " la varsta de " << varsta << " ani.\n";
                    std::cout << "  (Cost anual tratament: " << std::fixed << std::setprecision(2) << boliPosibile[idxBoala].getCostTratamentAnual() << "K)\n";
                    adaugaEveniment(varsta, "Boala Contractata", "A inceput tratamentul pentru " + boliPosibile[idxBoala].getNume());
                }
            }
        }
        if (this->stats.getBani() >= 1000000.0) {
            notifica("MILIARDAR");
        }

        if (varsta >= varstaDecesAleatorie || varsta >= VARSTA_MAXIMA_FORTATA) {
            marcheazaDeces("Batranete");
            return true;
        }
        varsta += 1;
        if (varsta == 18) {
            obtinePrimulJob();
        }
        verificaDecesParinti();
        if (verificaDeces()) return true;

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

void Personaj::afiseazaVerdictFinal() const {
    if (!getEsteMort()) {
        std::cout << "\n>>> SIMULAREA S-A OPRIT INAINTE DE MOARTE. Nu se poate emite un verdict final. <<<" << std::endl;
        return;
    }

    const double medieFericire = stats.getMedieFericireIstoric();

    std::cout << "\n=================================================" << std::endl;
    std::cout << ">>> VERDICT FINAL: VIATA LUI " << numeComplet << " <<<" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "Varsta de deces: " << varsta << " ani. Ultimul Eveniment Important: " << istoricEvenimente.back().getDescriere() << std::endl;
    std::cout << "Media istorica a Fericirii de-a lungul vietii: " << std::fixed << std::setprecision(1) << medieFericire << "%." << std::endl;

    std::string verdictViata;

    if (medieFericire >= 80.0) {
        verdictViata = "Viata Absolut Fericita (80-100%): O viata plina de bucurii, impliniri si relatii de nepretuit. A murit multumit si fara regrete.";
    } else if (medieFericire >= 60.0) {
        verdictViata = "Viata implinita (60-79%): O viata buna, echilibrata, cu provocari depasite si multe momente de satisfactie personala.";
    } else if (medieFericire >= 40.0) {
        verdictViata = "Viata Mediocra/Comuna (40-59%): O viata obisnuita, cu suisuri si coborasuri. A luptat, dar a avut parte si de frustrari semnificative.";
    } else if (medieFericire >= 20.0) {
        verdictViata = "Viata Trista/Dificila (20-39%): O viata marcata de suferinta, esecuri financiare sau pierderi majore. A murit cu multe regrete.";
    } else {
        verdictViata = "Viata Tragedie/Deprimanta (0-19%): O existenta dominată de nefericire profunda, izolare si probleme de sanatate nerezolvate.";
    }

    std::cout << "\nVERDICT NARATIV: " << verdictViata << std::endl;
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