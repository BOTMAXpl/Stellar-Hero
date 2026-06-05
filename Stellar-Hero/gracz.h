#ifndef GRACZ_H
#define GRACZ_H

#include "obiekt.h"


class Gracz : public Obiekt {
private:
    sf::Texture texture;
    sf::Sprite sprite;

    sf::RectangleShape shape;
    float speed;

    sf::Vector2f kierunek;

public:
    Gracz();
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    sf::Vector2f getkierunek() const { return kierunek; }
};

#endif
