#include "boss.h"
#include <cmath>

boss::boss(float start_x, float start_y) {
    pozycja.x = start_x;
    pozycja.y = start_y;
    zycia = 7;
    czas_strzalu = 0.0f;
    czas_ogolny = 0.0f;
    czas_niesmiertelnosci = 0.0f;
    faza_ataku = 0;
    strzal_gotowy = false;

    tekstura.loadFromFile("boss.png");
    tekstura.setSmooth(false);
    sprajt.setTexture(tekstura);

    // ustawainie srodka zeby lepiej dzialalo
    sprajt.setOrigin(tekstura.getSize().x / 2.0f, tekstura.getSize().y / 2.0f);
    sprajt.setPosition(pozycja);
    sprajt.setScale(1.5f, 1.5f);
}

void boss::licz(float dt) {
    czas_ogolny += dt;
    czas_strzalu += dt;

    if (czas_niesmiertelnosci > 0) {
        czas_niesmiertelnosci -= dt;
        sprajt.setColor(sf::Color(255, 100, 100));
    } else {
        sprajt.setColor(sf::Color::White);
    }

    pozycja.x = 960.0f + std::sin(czas_ogolny * 1.5f) * 600.0f;
    sprajt.setPosition(pozycja);

    if (czas_strzalu > 2.0f) {
        strzal_gotowy = true;
        faza_ataku = (faza_ataku == 0) ? 1 : 0;
        czas_strzalu = 0.0f;
    }
}

void boss::rysuj(sf::RenderWindow& okno) {
    okno.draw(sprajt);
}

bool boss::czy_usunac() {
    return zycia <= 0;
}

sf::FloatRect boss::daj_kolizje() {
    return sprajt.getGlobalBounds();
}

void boss::dostan_obrazenia() {
    if (czas_niesmiertelnosci <= 0.0f) {
        zycia--;
        czas_niesmiertelnosci = 0.5f;
    }
}

bool boss::gotowy_do_strzalu() {
    return strzal_gotowy;
}

void boss::zresetuj_strzal() {
    strzal_gotowy = false;
}

int boss::daj_faze() {
    return faza_ataku;
}

sf::Vector2f boss::daj_srodek() {
    return pozycja;
}
