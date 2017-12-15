#include <SFML/Graphics.hpp>
#include "matrix.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Candle in water simulation", sf::Style::Titlebar | sf::Style::Close);     

    Matrix matrix(50, 50);

    sf::Clock clock;
    while(window.isOpen())
    {
        sf::Time deltaTime = clock.restart();

        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed: window.close(); break;
                default: break;
            }
        }

        matrix.update(deltaTime);

        window.clear(sf::Color::White);
        window.draw(matrix);
        window.display();
    }

    return 0;
}
