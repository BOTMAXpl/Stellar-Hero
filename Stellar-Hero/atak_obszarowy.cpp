#include "atak_obszarowy.h"

atak_obszarowy::atak_obszarowy(sf::Vector2f srodek) {
    czas_zycia = 0;
    maks_czas = 0.6f;

    t1.loadFromFile("pole1.png");
    t2.loadFromFile("pole2.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    obrazek.setTexture(t1);

    obrazek.setOrigin(t1.getSize().x / 2.0f, t1.getSize().y / 2.0f);

    pozycja = srodek;
    obrazek.setPosition(pozycja);

    obrazek.setScale(1.0f, 1.0f);
}

void atak_obszarowy::licz(float dt) {
    czas_zycia += dt;

    if (czas_zycia > maks_czas / 2.0f) {
        obrazek.setTexture(t2, true);
        obrazek.setOrigin(t2.getSize().x / 2.0f, t2.getSize().y / 2.0f);
    }
}

void atak_obszarowy::rysuj(sf::RenderWindow& okno) {
    if (!czy_usunac()) okno.draw(obrazek);
}

bool atak_obszarowy::czy_usunac() {
    return czas_zycia >= maks_czas;
}

sf::FloatRect atak_obszarowy::daj_kolizje() {
    if (czas_zycia <= maks_czas / 2.0f) {
        return sf::FloatRect(0, 0, 0, 0);
    }

    return obrazek.getGlobalBounds();
}
