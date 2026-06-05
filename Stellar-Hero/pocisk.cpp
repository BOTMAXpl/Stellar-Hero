#include "pocisk.h"

Pocisk::Pocisk(sf::Vector2f startPos, sf::Vector2f kierunek) {
    speed = 800.0f;

    velocity = kierunek * speed;

    position = startPos;

    shape.setRadius(5.0f);
    shape.setFillColor(sf::Color::Yellow);

    shape.setOrigin(5.0f, 5.0f);
    shape.setPosition(position);
}

void Pocisk::update(float deltaTime) {
    position += velocity * deltaTime;
    shape.setPosition(position);
}

void Pocisk::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
