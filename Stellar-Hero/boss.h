#ifndef BOSS_H
#define BOSS_H
#include "obiekt.h"
#include <SFML/Graphics.hpp>

class boss : public obiekt {
private:
    sf::Texture tekstura;
    sf::Sprite sprajt;
    int zycia;
    float czas_strzalu;
    float czas_ogolny;
    float czas_niesmiertelnosci;
    int faza_ataku;
    bool strzal_gotowy;

public:
    boss(float start_x, float start_y);
    void licz(float dt) override;
    void rysuj(sf::RenderWindow& okno) override;
    bool czy_usunac() override;
    sf::FloatRect daj_kolizje() override;

    void dostan_obrazenia();
    bool gotowy_do_strzalu();
    void zresetuj_strzal();
    int daj_faze();
    sf::Vector2f daj_srodek();
};
#endif
