#ifndef OSTRZEZENIE_H
#define OSTRZEZENIE_H
#include "obiekt.h"

class ostrzezenie : public obiekt {
private:
    sf::Texture tekstura;
    sf::Sprite sprajt;
    float czas_zycia;
    int typ_wroga;

public:
    ostrzezenie(float start_x, float start_y, int typ);
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    bool czy_usunac() override;
    sf::FloatRect daj_kolizje() override;

    int daj_typ() { return typ_wroga; }
};
#endif
