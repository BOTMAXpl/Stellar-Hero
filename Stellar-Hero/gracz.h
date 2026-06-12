#ifndef GRACZ_H
#define GRACZ_H
#include "obiekt.h"

class gracz : public obiekt {
private:
    sf::Texture tekstura;
    sf::Sprite sprajt;
    float predkosc;
    sf::Vector2f kierunek_patrzenia;

    int zycia;
    float niesmiertelnosc;
    int numer_gracza;
    float czas_odnowienia_obszarowki;
public:
    gracz(int numer = 1);
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;

    sf::Vector2f wez_kierunek() { return kierunek_patrzenia; }

    sf::FloatRect daj_kolizje() override;
    void dostan_obrazenia();
    int ile_zyc() { return zycia; }
    void zresetuj();
    bool gotowy_obszarowy() { return czas_odnowienia_obszarowki <= 0; }
    void uzyj_obszarowego() { czas_odnowienia_obszarowki = 5.0f; }
};
#endif
