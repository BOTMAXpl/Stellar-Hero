#include <iostream>
#include <SFML/Graphics.hpp>
#include "Gracz.h"
#include "pocisk.h"
using namespace std;
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Stellar-Hero", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(120);

    vector<Obiekt*> obiekty;
    Gracz* bohater1 = new Gracz();
    obiekty.push_back(bohater1);

    sf::Clock clock;

    bool isSpacePressed = false;
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!isSpacePressed) {
                obiekty.push_back(new Pocisk(bohater1->getPosition(), bohater1->getkierunek()));
                isSpacePressed = true;
            }
        } else {
            isSpacePressed = false;
        }

        for (auto obiekt : obiekty) {
            obiekt->update(deltaTime);
        }

        window.clear(sf::Color(0,0,0));

        for (auto obiekt : obiekty) {
            obiekt->draw(window);
        }

        window.display();
    }

    for (auto obiekt : obiekty) {
        delete obiekt;
    }

    return 0;
}
