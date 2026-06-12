#include "wrog.h"
#include <cstdlib>
#include <cmath>

wrog::wrog(float start_x, float start_y) {
    pozycja.x = start_x;
    pozycja.y = start_y;
    zyje = true;

    tekstura.loadFromFile("wrog1.png");
    tekstura.setSmooth(false);

    sprajt.setTexture(tekstura);
    sprajt.setPosition(pozycja);
    sprajt.setScale(1.0f, 1.0f);

    predkosc = 150.0f;
    czas_do_zmiany_ruchu = 0.0f;
    czas_do_strzalu = 2.0f;
}

void wrog::licz(float dt) {
    czas_do_zmiany_ruchu -= dt;
    czas_do_strzalu -= dt;

    if (czas_do_zmiany_ruchu <= 0) {
        float rx = (rand() % 3) - 1.0f;
        float ry = (rand() % 3) - 1.0f;

        float dlugosc = std::sqrt(rx * rx + ry * ry);
        if (dlugosc != 0) {
            kierunek_ruchu.x = rx / dlugosc;
            kierunek_ruchu.y = ry / dlugosc;
        } else {
            kierunek_ruchu = sf::Vector2f(0, 0);
        }
        czas_do_zmiany_ruchu = 1.0f + (rand() % 3);
    }

    pozycja.x += kierunek_ruchu.x * predkosc * dt;
    pozycja.y += kierunek_ruchu.y * predkosc * dt;

    if (pozycja.x < 100) { pozycja.x = 100; kierunek_ruchu.x *= -1; }
    if (pozycja.x > 1800) { pozycja.x = 1800; kierunek_ruchu.x *= -1; }
    if (pozycja.y < 100) { pozycja.y = 100; kierunek_ruchu.y *= -1; }
    if (pozycja.y > 1000) { pozycja.y = 1000; kierunek_ruchu.y *= -1; }

    sprajt.setPosition(pozycja);
}

bool wrog::gotowy_do_strzalu() {
    return czas_do_strzalu <= 0;
}

void wrog::zresetuj_strzal() {
    czas_do_strzalu = 2.0f + (rand() % 3);
}


void wrog::rysuj(sf::RenderWindow& okno) {
    if (zyje == true) okno.draw(sprajt);
}
bool wrog::czy_usunac() {
    return !zyje;
}
sf::FloatRect wrog::daj_kolizje() {
    return sprajt.getGlobalBounds();
}
void wrog::zabij() {
    zyje = false;
}
sf::Vector2f wrog::daj_srodek() {
    sf::FloatRect wymiary = sprajt.getGlobalBounds();
    return sf::Vector2f(wymiary.left + wymiary.width / 2.0f, wymiary.top + wymiary.height / 2.0f);
}
