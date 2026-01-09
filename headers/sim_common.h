#ifndef SIM_COMMON_H
#define SIM_COMMON_H

#include <string>
#include <vector>
#include <iomanip>
#include "SimUtilities.h"

constexpr int VAL_MIN_STAT = 0;
constexpr int VAL_MAX_STAT = 100;
constexpr int VITEZA_DEGRADARE_FERICIRE = 1;
constexpr int PRAG_SANATATE_CRITICA = 5;
constexpr int MAX_OPTIUNI_DECIZIE = 14;
constexpr int VARSTA_MAXIMA_FORTATA = 100;
constexpr int STAT_VIZIBILITATE_INIT = 10;
constexpr int STAT_VIZIBILITATE_MAX = 100;
constexpr int BANI_MAX_INIT= 999999999;
constexpr int VALOARE_BANI_INITIALA=0;

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

struct DestinatieVacanta {
    std::string nume;
    double costBaza;
    int bonusFericire;
};

struct DataNastere {
    int zi=0;
    int luna=0;
    const int an = 2025;
};

struct Job {
    std::string nume;
    int salariuAnual;
    int satisfactie;
    int cerintaInteligenta;
};

const std::vector<DestinatieVacanta> DESTINATII_VACANTA = {
    {"Litoralul Romanesc", 3.0, 10},
    {"Camping in Muntii Carpati", 1.5, 12},
    {"Capitala Europeana (City Break)", 10.0, 20},
    {"Excursie Exotica (Insula)", 35.0, 40},
    {"Vacanta de Lux (Circuit Asia)", 80.0, 50}
};

inline std::string alegeNumeRandom(const bool eBarbat) {
    if (eBarbat) {
        return alegeElementAleatoriu(NUME_BARBATI);
    }
    return alegeElementAleatoriu(NUME_FEMEI);
}

std::string getLunaString(int luna);
DataNastere genereazaDataNastere();
std::ostream& operator<<(std::ostream& os, const DataNastere& dn);

class Statistici;
class Relatie;
class Cariera;

#endif // SIM_COMMON_H