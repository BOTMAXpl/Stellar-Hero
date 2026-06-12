#include "laser_wroga.h"
#include <cmath>

laser_wroga::laser_wroga(sf::Vector2f start, sf::Vector2f kierunek) {
    czas_zycia = 0;
    maks_czas = 7.0f;

    t1.loadFromFile("laserwroga1.png");
    t2.loadFromFile("laserwroga2.png");
    t1.setSmooth(true);
    t2.setSmooth(true);

    obrazek.setTexture(t1);
    obrazek.setOrigin(0, t1.getSize().y / 2.0f);

    pozycja = start;
    obrazek.setPosition(pozycja);

    float kat = std::atan2(kierunek.y, kierunek.x) * 180 / 3.14159;
    obrazek.setRotation(kat);
}

void laser_wroga::licz(float dt) {
    czas_zycia += dt;
    if (czas_zycia > maks_czas / 2.0f) {
        obrazek.setTexture(t2, true);
        obrazek.setOrigin(0, t2.getSize().y / 2.0f);
    }
}

void laser_wroga::rysuj(sf::RenderWindow& okno) {
    if (!czy_usunac()) okno.draw(obrazek);
}

bool laser_wroga::czy_usunac() {
    return czas_zycia >= maks_czas;
}

bool laser_wroga::czy_trafia(sf::Vector2f punkt_gracza) {
    if (czas_zycia <= maks_czas / 2.0f) {
        return false;
    }
    sf::Vector2f punkt_lokalny = obrazek.getInverseTransform().transformPoint(punkt_gracza);
    sf::FloatRect granice = obrazek.getLocalBounds();

    granice.left -= 30;
    granice.top -= 30;
    granice.width += 60;
    granice.height += 60;

    return granice.contains(punkt_lokalny);
}
void laser_wroga::zniszcz() {
    czas_zycia = maks_czas;
}

sf::Vector2f laser_wroga::wez_pozycje_startowa() {
    return pozycja;
}
