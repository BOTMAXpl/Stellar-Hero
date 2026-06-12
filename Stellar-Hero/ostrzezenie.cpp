#include "ostrzezenie.h"

ostrzezenie::ostrzezenie(float start_x, float start_y, int typ) {
    pozycja.x = start_x;
    pozycja.y = start_y;
    typ_wroga = typ;
    czas_zycia = 1.5f;

    tekstura.loadFromFile("kolo1.png");
    tekstura.setSmooth(false);

    sprajt.setTexture(tekstura);
    sprajt.setOrigin(tekstura.getSize().x / 2.0f, tekstura.getSize().y / 2.0f);
    sprajt.setPosition(pozycja);
    sprajt.setScale(1.0f, 1.0f);
}

void ostrzezenie::licz(float dt) {
    czas_zycia -= dt;
}

void ostrzezenie::rysuj(sf::RenderWindow& okno) {
    okno.draw(sprajt);
}

bool ostrzezenie::czy_usunac() {
    return czas_zycia <= 0;
}

sf::FloatRect ostrzezenie::daj_kolizje() {
    return sf::FloatRect(0, 0, 0, 0);
}
