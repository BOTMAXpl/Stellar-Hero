#ifndef OBIEKT_H
#define OBIEKT_H
#include <SFML/Graphics.hpp>

class Obiekt {
protected:
    sf::Vector2f position;

public:
    Obiekt();
    virtual ~Obiekt() = default;
    virtual void update(float deltaTime) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    void setPosition(float x, float y) { position.x = x; position.y = y; }
    sf::Vector2f getPosition() const { return position; }
};

#endif
