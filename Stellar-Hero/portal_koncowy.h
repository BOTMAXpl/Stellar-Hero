#ifndef PORTAL_KONCOWY_H
#define PORTAL_KONCOWY_H
#include "obiekt.h"
class portal_koncowy : public obiekt {
private:
    sf::CircleShape ksztalt;
public:
    portal_koncowy(float x, float y) {
        pozycja.x = x; pozycja.y = y;
        ksztalt.setRadius(60);
        ksztalt.setOrigin(60, 60);
        ksztalt.setPosition(pozycja);
        ksztalt.setFillColor(sf::Color(150, 0, 255, 200));
        ksztalt.setOutlineThickness(8);
        ksztalt.setOutlineColor(sf::Color::White);
    }
    void licz(float dt) override {}
    void rysuj(sf::RenderWindow& okno) override { okno.draw(ksztalt); }
    bool czy_usunac() override { return false; }
    sf::FloatRect daj_kolizje() override { return ksztalt.getGlobalBounds(); }
};
#endif