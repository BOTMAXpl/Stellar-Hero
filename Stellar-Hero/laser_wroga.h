#ifndef LASER_WROGA_H
#define LASER_WROGA_H
#include "obiekt.h"

class laser_wroga : public obiekt {
private:
    sf::Texture t1;
    sf::Texture t2;
    sf::Sprite obrazek;

    float czas_zycia;
    float maks_czas;

public:
    laser_wroga(sf::Vector2f start, sf::Vector2f kierunek);
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    bool czy_usunac() override;

    bool czy_trafia(sf::Vector2f punkt_gracza);
};

#endif
