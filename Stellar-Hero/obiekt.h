#ifndef OBIEKT_H
#define OBIEKT_H
#include <SFML/Graphics.hpp>

class obiekt {
protected:
    sf::Vector2f pozycja;

public:
    obiekt() { pozycja.x = 0; pozycja.y = 0; }
    virtual ~obiekt() {}

    virtual void licz(float dt) = 0;
    virtual void rysuj(sf::RenderWindow& okno) = 0;

    virtual bool czy_usunac() {return false;}

    sf::Vector2f daj_pozycje() {return pozycja;}

    virtual sf::FloatRect daj_kolizje() {return sf::FloatRect(0, 0, 0, 0);}
};
#endif
