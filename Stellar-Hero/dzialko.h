#ifndef DZIALKO_H
#define DZIALKO_H
#include "obiekt.h"

class dzialko : public obiekt {
private:
    sf::Texture tekstura;
    sf::Sprite sprajt;
    bool zyje;
    float czas_do_strzalu;

public:
    dzialko(float start_x, float start_y);

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
