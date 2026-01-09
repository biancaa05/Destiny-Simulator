#ifndef SISTEM_REALIZARI_H
#define SISTEM_REALIZARI_H

#include "Observer.h"
#include <iostream>
#include <string>
#include <set>

class SistemRealizari final : public Observer {
private:
    std::set<std::string> realizariDeblocate;

public:
    void onNotify(const std::string& eveniment) override {
        std::string mesaj;

        if (eveniment == "CASATORIE") {
            mesaj = "Familia pe primul loc (Te-ai casatorit!)";
        }
        else
            if (eveniment == "PENSIONARE") {
                mesaj = "O viata de munca implinita (Te-ai pensionat!)";
            }
            else
                if (eveniment == "PRIMUL_COPIL") {
                    mesaj = "Mostenirea continua (Ai avut primul copil!)";
                }
                else
                    if (eveniment == "INCHISOARE") {
                        mesaj = "Viata de interlop (Ai ajuns la inchisoare!)";
                    }
                    else
                        if (eveniment == "MILIARDAR") {
                            mesaj = "Lupul de pe Wall Street (Ai atins 1.000.000.000 lei!)";
                        }

        if (!mesaj.empty()) {
            std::cout << "\n   [ACHIEVEMENT UNLOCKED] - " << mesaj << "\n";
            realizariDeblocate.insert(mesaj);
        }
    }

    void afiseazaRealizariFinale() const {
        std::cout << "\n=================================================" << std::endl;
        std::cout << "      COLECTIA TA DE REALIZARI" << std::endl;
        std::cout << "=================================================" << std::endl;

        if (realizariDeblocate.empty()) {
            std::cout << "Nicio realizare deblocata. O viata cam banala..." << std::endl;
        } else {
            for (const auto& r : realizariDeblocate) {
                std::cout << " [X] " << r << std::endl;
            }
            std::cout << "\nTotal: " << realizariDeblocate.size() << " realizari." << std::endl;
        }
        std::cout << "=================================================\n" << std::endl;
    }
};

#endif