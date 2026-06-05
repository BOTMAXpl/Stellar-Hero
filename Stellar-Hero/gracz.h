#ifndef GRACZ_H
#define GRACZ_H

#include "obiekt.h"


class Gracz : public Obiekt {
private:
    sf::RectangleShape shape;
    float speed;

public:
    Gracz();
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;
};

#endif
