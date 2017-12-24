#include <SFML/Graphics.hpp>
#include "candle.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Candle in water simulation", sf::Style::Titlebar | sf::Style::Close);     

	sf::Clock clock;
	Candle candle;

	const auto center = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

	sf::RectangleShape water(sf::Vector2f(window.getSize().x, center.y));
	water.setPosition(0, center.y);
	water.setFillColor(getColorByMaterial(Water));

	candle.setPosition(center.x, center.y + candle.getSizeInPx().y - 9 * pixelsPerUnit);

    while(window.isOpen())
    {
	    const auto delta_time = clock.restart();

		// Events handling
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed: window.close(); break;
                default: break;
            }
        }

		// Update
		/*const auto candle_offset = (center.y) + (getDensityByMaterial(Water)/candle.CalculateAverageDensity()) * (candle.getSizeInPx().y - 9 * pixelsPerUnit);
		candle.SetWaterLevel(candle_offset - center.y);
		candle.setPosition(center.x, candle_offset + 9 * pixelsPerUnit);*/
		//TODO: Candle floating
		candle.update(delta_time);

		// Drawning
        window.clear(sf::Color::White);
		window.draw(water);
        window.draw(candle);
        window.display();
    }

    return 0;
}
