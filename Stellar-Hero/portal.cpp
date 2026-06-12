#include "portal.h"

portal::portal(float start_x, float start_y) {
    pozycja.x = start_x;
    pozycja.y = start_y;
    czas_animacji = 0;
    klatka = 1;

    t1.loadFromFile("portal1.png");
    t2.loadFromFile("portal2.png");
    t3.loadFromFile("portal3.png");

    t1.setSmooth(false);
    t2.setSmooth(false);
    t3.setSmooth(false);

    sprajt.setTexture(t1);

    sprajt.setOrigin(t1.getSize().x / 2.0f, t1.getSize().y / 2.0f);
    sprajt.setPosition(pozycja);
    sprajt.setScale(3.0f, 3.0f);
}

void portal::licz(float dt) {
    czas_animacji += dt;


    if (czas_animacji > 0.15f) {
        czas_animacji = 0;
        klatka++;
        if (klatka > 3) klatka = 1;
        if (klatka == 1) sprajt.setTexture(t1, true);
        if (klatka == 2) sprajt.setTexture(t2, true);
        if (klatka == 3) sprajt.setTexture(t3, true);
    }
}

void portal::rysuj(sf::RenderWindow& okno) {
    okno.draw(sprajt);
}

sf::FloatRect portal::daj_kolizje() {
    return sprajt.getGlobalBounds();
}

bool portal::czy_usunac() {
    return false;
}