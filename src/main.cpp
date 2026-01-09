#include <fstream>
#include <random>
#include "../headers/Personaj.h"
#include "../headers/Statistici.h"
#include "../headers/Exceptii.h"
#include "../headers/GeneratorRandom.h"
#include "../headers/SistemRealizari.h"

bool citesteDateIntrare(std::string& nume, std::string& prenume, std::string& nationalitate, int& aniSimulare, std::vector<int>& decizii_anuale) {
    std::ifstream fisierTastatura("tastatura.txt");

    decizii_anuale.clear();
    nume = "";
    aniSimulare = 0;

    if (!fisierTastatura.is_open()) {
         throw EroareFisier("tastatura.txt");
    }

    if (!(fisierTastatura >> nume >> prenume >> nationalitate)) {
         throw EroareSimulareDurata();
    }

    if (!(fisierTastatura >> aniSimulare) || aniSimulare <= 0) {
         throw EroareSimulareDurata();
    }

    int decizie;
    for (int i = 0; i < aniSimulare; ++i) {
        if (fisierTastatura >> decizie) {
            if (decizie < 0 || decizie > MAX_OPTIUNI_DECIZIE) {
                throw EroareDecizieInvalida(decizie);
            }
            decizii_anuale.push_back(decizie);
        }
        else {
             throw EroareSimulareDurata();
        }
    }

    if (decizii_anuale.size() != static_cast<size_t>(aniSimulare)) {
         throw EroareSimulareDurata();
    }

    fisierTastatura.close();
    return true;
}



std::string getLunaString(const int luna) {
    static const std::vector<std::string> numeLuni = {
        "", "Ianuarie", "Februarie", "Martie", "Aprilie", "Mai", "Iunie",
        "Iulie", "August", "Septembrie", "Octombrie", "Noiembrie", "Decembrie"
    };
    return numeLuni[luna];
}

DataNastere genereazaDataNastere() {
    DataNastere dn;
    dn.luna = GeneratorRandom::getInstance().getRandomInt(1, 12);
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
    dn.zi = GeneratorRandom::getInstance().getRandomInt(1, zile_max);
    return dn;
}
std::ostream& operator<<(std::ostream& os, const DataNastere& dn) {
    os << dn.zi << " " << getLunaString(dn.luna) << " " << dn.an;
    return os;
}

int main() {

    std::cout << "--- DESTINY SIMULATOR ---" << std::endl;

    Personaj* jucatorPtr = nullptr;

    try {
        std::string nume, prenume, nationalitate;
        int ani_simulare = 0;
        std::vector<int> decizii_anuale;
        citesteDateIntrare(nume, prenume, nationalitate, ani_simulare, decizii_anuale);

        const std::string numeComplet = nume + " " + prenume;
        const DataNastere dn = genereazaDataNastere();

        constexpr int MIN_HIGH_STAT = 80;
        constexpr int MAX_HIGH_STAT = 95;

        const Statistici stats_initiale(
            GeneratorRandom::getInstance().getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT),
            GeneratorRandom::getInstance().getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT),
            GeneratorRandom::getInstance().getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT),
            GeneratorRandom::getInstance().getRandomInt(MIN_HIGH_STAT, MAX_HIGH_STAT),
            STAT_VIZIBILITATE_INIT,
            VALOARE_BANI_INITIALA
        );

        jucatorPtr = new Personaj(numeComplet, nationalitate, 1, dn, stats_initiale);

        Personaj jucator_copie = *jucatorPtr;
        jucator_copie = *jucatorPtr;

        std::cout << "[TEST LOG] Copia varstei (pentru a suprima warning-ul): " << jucator_copie.getVarsta() << std::endl;
        SistemRealizari achievementSystem;
        jucatorPtr->adaugaObservator(&achievementSystem);
        std::cout << "\n--- START VIATA ---" << std::endl;

        for (int i1 = 0; i1 < ani_simulare; ++i1) {
            if (jucatorPtr->getEsteMort()) break;

            if (jucatorPtr->esteInInchisoare()) {
                std::cout << "\n--- ANUL " << jucatorPtr->getVarsta() << " (INCHISOARE) ---" << std::endl;
                std::cout << "[SIMULARE] Decizia din fisier (" << decizii_anuale[i1] << ") este IGNORATA.\n";

                if (jucatorPtr->aplicaAnual(1)) break;

                continue;
            }

            std::cout << "\n--- STARE CURENTA INAINTE DE DECIZIA ANULUI " << jucatorPtr->getVarsta() << " ---" << std::endl;
            std::cout << *jucatorPtr << std::endl;

            const int alegere = decizii_anuale[i1];

            Personaj::afiseazaMeniuDecizie();
            std::cout << "\n> ACTIUNE JUCATOR (DIN FISIER): Se executa decizia " << alegere << " pentru anul " << jucatorPtr->getVarsta()<< "..." << std::endl;

            jucatorPtr->iaDecizieDestin(alegere);
            if (jucatorPtr->getEsteMort()) break;

            if (jucatorPtr->aplicaAnual(1)) break;
        }

        std::cout << "\n=================================================" << std::endl;
        std::cout << "  JOC TERMINAT. REZULTAT FINAL." << std::endl;
        std::cout << "=================================================" << std::endl;

        jucatorPtr->afiseazaIstoricViata();
        jucatorPtr->afiseazaVerdictFinal();
        achievementSystem.afiseazaRealizariFinale();

        if (!jucatorPtr->getEsteMort()) {
            std::cout << *jucatorPtr << std::endl;
        }

        delete jucatorPtr;

    }
    catch (const EroareFisier& e) {
        std::cerr << "\n[EROARE FATALA: I/O] " << e.what() << std::endl;
        delete jucatorPtr;
        return 1;
    }
    catch (const EroareDecizieInvalida& e) {
        std::cerr << "\n[EROARE DECIZIE] " << e.what() << std::endl;
        delete jucatorPtr;
        return 1;
    }
    catch (const EroareSimulareDurata& e)   {
        std::cerr << "\n[EROARE STRUCTURA DATE] " << e.what() << std::endl;
        delete jucatorPtr;
        return 1;
    }
    catch (const EroareSimulare& e) {
        std::cerr << "\n[EROARE SIMULARE JOC] " << e.what() << std::endl;
        delete jucatorPtr;
        return 1;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "\n[EROARE CRITICA]: Eroare la accesarea listelor: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "\n[EROARE NECUNOSCUTA] O exceptie de sistem a aparut: " << e.what() << std::endl;
        delete jucatorPtr;
        return 1;
    }
    catch (...) {
        std::cerr << "\n[EROARE NECUNOSCUTA] O exceptie neidentificata a fost interceptata." << std::endl;
        delete jucatorPtr;
        return 1;
    }

    std::cout << "[FINAL LOG] Numar personaje active (dupa delete): " << Personaj::getNrPersonajeActive() << std::endl;
    return 0;
}