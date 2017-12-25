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

	candle.setPosition(center.x, center.y + candle.getSizeInPx().y * units::pixelsPerUnit);
	
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
		
    	// Candle floating
		const auto candle_offset = (candle.CalculateAverageDensity() / getDensity(Water)) * candle.getSizeInPx().y;
		candle.SetWaterLevel(candle_offset);
		candle.setPosition(center.x, std::min(center.y + candle_offset, static_cast<double>(window.getSize().y)));

		// Update
		candle.update(delta_time);
		
		// Drawning
        window.clear(sf::Color::White);
		window.draw(water);
        window.draw(candle);
        window.display();
    }

    return 0;
}
