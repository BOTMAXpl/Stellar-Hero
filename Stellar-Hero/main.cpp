#include <iostream>
#include <SFML/Graphics.hpp>
#include "Gracz.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Stellar-Hero", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(120);

    Gracz bohater1;

    sf::Clock clock;
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        bohater1.update(deltaTime);
        window.clear(sf::Color(0,0,0));
        bohater1.draw(window);
        window.display();
    }

    return 0;
}
