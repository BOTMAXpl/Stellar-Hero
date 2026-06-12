#ifndef ATAK_OBSZAROWY_H
#define ATAK_OBSZAROWY_H
#include "obiekt.h"

class atak_obszarowy : public obiekt {
private:
    sf::Texture t1;
    sf::Texture t2;
    sf::Sprite obrazek;
    float czas_zycia;
    float maks_czas;

public:
    atak_obszarowy(sf::Vector2f srodek);
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    bool czy_usunac() override;
    sf::FloatRect daj_kolizje() override;
};

#endif
