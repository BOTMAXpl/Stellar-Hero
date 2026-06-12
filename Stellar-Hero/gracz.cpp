#include "gracz.h"
#include <iostream>
#include <cmath>

gracz::gracz(int numer) {
    numer_gracza = numer;
    if (numer_gracza == 1) {
        pozycja = sf::Vector2f(600, 360);
        predkosc = 300;
    }
    else {
        pozycja = sf::Vector2f(680, 360);
        predkosc = 600;
    }
    zycia = 3;
    niesmiertelnosc = 0;
    czas_odnowienia_obszarowki = 0;

    pozycja = sf::Vector2f(640, 360);
    kierunek_patrzenia = sf::Vector2f(0, 1);

    if (numer_gracza == 1) {
        tekstura.loadFromFile("Frieren1.png");
    } else {
        tekstura.loadFromFile("postac2.png");
    }
    tekstura.setSmooth(false);

    sprajt.setTexture(tekstura);
    sprajt.setScale(1, 1);
    sprajt.setPosition(pozycja);
}

void gracz::licz(float dt) {
    if (czas_odnowienia_obszarowki > 0) {
        czas_odnowienia_obszarowki -= dt;
    }
    if (niesmiertelnosc > 0) {
        niesmiertelnosc -= dt;
        if ((int)(niesmiertelnosc * 10) % 2 == 0) {
            sprajt.setColor(sf::Color(255, 255, 255, 100));
        } else {
            sprajt.setColor(sf::Color(255, 255, 255, 255));
        }
    } else {
        sprajt.setColor(sf::Color(255, 255, 255, 255));
    }
    sf::Vector2f ruch(0, 0);
    if (numer_gracza == 1) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ruch.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ruch.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) ruch.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ruch.x += 1;
    } else if (numer_gracza == 2) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ruch.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ruch.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ruch.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ruch.x += 1;
    }

    float dlugosc = std::sqrt(ruch.x * ruch.x + ruch.y * ruch.y);
    if (dlugosc != 0) {
        ruch.x = ruch.x / dlugosc;
        ruch.y = ruch.y / dlugosc;
        kierunek_patrzenia = ruch;
    }

    pozycja.x += ruch.x * predkosc * dt;
    pozycja.y += ruch.y * predkosc * dt;

    if (pozycja.x < 50) pozycja.x = 50;
    if (pozycja.x > 1820) pozycja.x = 1820;
    if (pozycja.y < 60) pozycja.y = 60;
    if (pozycja.y > 960) pozycja.y = 960;

    sprajt.setPosition(pozycja);
}

void gracz::rysuj(sf::RenderWindow& okno) {
    okno.draw(sprajt);
}

sf::FloatRect gracz::daj_kolizje() {
    return sprajt.getGlobalBounds();
}

void gracz::dostan_obrazenia() {
    if (niesmiertelnosc <= 0) {
        zycia--;
        niesmiertelnosc = 1.5f;
        std::cout << "Zostalo zyc: " << zycia << std::endl;
    }
}

void gracz::zresetuj() {
    zycia = 3;
    pozycja = sf::Vector2f(640, 360);
    niesmiertelnosc = 0;
}
