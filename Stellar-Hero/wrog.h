#ifndef WROG_H
#define WROG_H
#include "obiekt.h"

class wrog : public obiekt {
private:
    sf::Texture tekstura;
    sf::Sprite sprajt;
    bool zyje;

    float predkosc;
    sf::Vector2f kierunek_ruchu;
    float czas_do_zmiany_ruchu;
    float czas_do_strzalu;

public:
    wrog(float start_x, float start_y);

    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    bool czy_usunac() override;

    sf::FloatRect daj_kolizje() override;
    void zabij();
    sf::Vector2f daj_srodek();

    bool gotowy_do_strzalu();
    void zresetuj_strzal();
};
#endif
