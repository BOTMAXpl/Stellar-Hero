#ifndef PORTAL_H
#define PORTAL_H
#include "obiekt.h"

class portal : public obiekt {
private:
    sf::Texture t1, t2, t3;
    sf::Sprite sprajt;
    float czas_animacji;
    int klatka;

public:
    portal(float start_x, float start_y);
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    sf::FloatRect daj_kolizje() override;
    bool czy_usunac() override;
};
#endif
