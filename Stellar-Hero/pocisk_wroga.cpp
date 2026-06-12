#include "pocisk_wroga.h"
#include <cmath>

pocisk_wroga::pocisk_wroga(sf::Vector2f start, sf::Vector2f kierunek) {
    pozycja = start;
    kierunek_lotu = kierunek;
    predkosc = 400.0f;
    czas_zycia = 0;

    tekstura.loadFromFile("pocisk1.png");
    tekstura.setSmooth(false);
    obrazek.setTexture(tekstura);


    obrazek.setScale(1.0f, 1.0f);
    obrazek.setOrigin(tekstura.getSize().x / 2.0f, tekstura.getSize().y / 2.0f);
    obrazek.setPosition(pozycja);


    float kat = std::atan2(kierunek_lotu.y, kierunek_lotu.x) * 180 / 3.14159;
    obrazek.setRotation(kat);
}

void pocisk_wroga::licz(float dt) {
    czas_zycia += dt;
    pozycja.x += kierunek_lotu.x * predkosc * dt;
    pozycja.y += kierunek_lotu.y * predkosc * dt;
    obrazek.setPosition(pozycja);
}

void pocisk_wroga::rysuj(sf::RenderWindow& okno) {
    if (czy_usunac() == false) {
        okno.draw(obrazek);
    }
}

bool pocisk_wroga::czy_usunac() {
    if (czas_zycia >= 5.0f) return true;
    return false;
}

sf::FloatRect pocisk_wroga::daj_kolizje() {
    return obrazek.getGlobalBounds();
}

void pocisk_wroga::zniszcz() {
    czas_zycia = 10.0f;
}
