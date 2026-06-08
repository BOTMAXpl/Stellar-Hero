#include "gracz.h"
#include <cmath>
#include <iostream>

Gracz::Gracz() {
    speed = 123.0f;

    kierunek = sf::Vector2f(0.0f, 1.0f);

    position = sf::Vector2f(640.0f, 360.0f);
    if (!texture.loadFromFile("Frieren1.png")) {
        std::cerr << "Blad: Nie mozna zaladowac tekstury gracza!" << std::endl;
    }

    texture.setSmooth(false);

    sprite.setTexture(texture);

    sprite.setScale(3.0f, 3.0f);

    sprite.setPosition(position);
}

void Gracz::update(float deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.0f;

    float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
    if (length != 0.0f) {
        movement /= length;
        kierunek = movement;
    }
    position += movement * speed * deltaTime;

    sprite.setPosition(position);
}

void Gracz::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
