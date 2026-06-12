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
public:
    gracz();
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;

    sf::Vector2f wez_kierunek() { return kierunek_patrzenia; }

    sf::FloatRect daj_kolizje() override;
    void dostan_obrazenia();
    int ile_zyc() { return zycia; }
    void zresetuj();
};
#endif
