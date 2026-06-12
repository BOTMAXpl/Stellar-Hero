#ifndef POCISK_WROGA_H
#define POCISK_WROGA_H
#include "obiekt.h"

class pocisk_wroga : public obiekt {
private:
    sf::Texture tekstura;
    sf::Sprite obrazek;
    sf::Vector2f kierunek_lotu;
    float predkosc;
    float czas_zycia;

public:
    pocisk_wroga(sf::Vector2f start, sf::Vector2f kierunek);

    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    bool czy_usunac() override;

    sf::FloatRect daj_kolizje() override;
    void zniszcz();
};
#endif
