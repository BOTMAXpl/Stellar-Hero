#ifndef POCISK_H
#define POCISK_H

#include "obiekt.h"

class Pocisk : public Obiekt {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;

public:
    Pocisk(sf::Vector2f startPos, sf::Vector2f kierunek);

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};
#endif
