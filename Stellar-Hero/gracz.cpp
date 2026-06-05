#include "gracz.h"

Gracz::Gracz() {
    speed = 100.0f;

    shape.setSize(sf::Vector2f(50.0f, 50.0f));
    shape.setFillColor(sf::Color::Green);

    position = sf::Vector2f(640.0f, 360.0f);
    shape.setPosition(position);
}

void Gracz::update(float deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.0f;

    position += movement * speed * deltaTime;

    shape.setPosition(position);
}

void Gracz::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
