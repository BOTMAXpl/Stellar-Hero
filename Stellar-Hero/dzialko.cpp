#include "dzialko.h"
#include <cstdlib>

dzialko::dzialko(float start_x, float start_y) {
    pozycja.x = start_x;
    pozycja.y = start_y;
    zyje = true;

    tekstura.loadFromFile("wieza2.png");
    tekstura.setSmooth(false);

    sprajt.setTexture(tekstura);
    sprajt.setPosition(pozycja);
    sprajt.setScale(3.0f, 3.0f);

    sprajt.setColor(sf::Color(255, 100, 255));

    czas_do_strzalu = 3.0f;
}

void dzialko::licz(float dt) {
    czas_do_strzalu -= dt;
}

void dzialko::rysuj(sf::RenderWindow& okno) {
    if (zyje) okno.draw(sprajt);
}

bool dzialko::czy_usunac() {
    return !zyje;
}

sf::FloatRect dzialko::daj_kolizje() {
    return sprajt.getGlobalBounds();
}

void dzialko::zabij() {
    zyje = false;
}

sf::Vector2f dzialko::daj_srodek() {
    sf::FloatRect wymiary = sprajt.getGlobalBounds();
    return sf::Vector2f(wymiary.left + wymiary.width / 2.0f, wymiary.top + wymiary.height / 2.0f);
}

bool dzialko::gotowy_do_strzalu() {
    return czas_do_strzalu <= 0;
}

void dzialko::zresetuj_strzal() {
    czas_do_strzalu = 7.0f + (rand() % 2);
}
