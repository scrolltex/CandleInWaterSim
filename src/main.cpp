#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include "candle.hpp"
#include "gui.hpp"
#include "config.hpp"

float speedMultiplier = 900;

void CalculateCandleFloating(Candle &candle, sf::Vector2u wnd_size)
{
    const auto waterDensity = Config::Instance().Materials.at(Water).density;
	const auto center = sf::Vector2f(wnd_size.x / 2.0, wnd_size.y / 2.0);
	const auto candle_offset = (candle.CalculateAverageDensity() / waterDensity) * candle.GetSizeInPx().y;
	candle.SetWaterLevel(candle_offset);
	candle.setPosition(center.x, std::min(center.y + candle_offset, static_cast<double>(wnd_size.y)));
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Candle in water simulation"/*, sf::Style::Titlebar | sf::Style::Close*/);
	window.setFramerateLimit(60);
    tgui::Gui gui(window);

    try
    {
        gui::CreateGUI(gui);
    }
    catch(const tgui::Exception &e)
    {
        std::cerr << "Failed to create TGUI widgets: " << e.what() << std::endl;
        return 1;
    }
	
	// Register callbacks for gui
    gui::onApplyCallback = [&] {
		Config::Instance().isPlaying = false;

        Candle::Instance().SetSize(Config::Instance().CandleSize);
    };

	auto center = sf::Vector2f(window.getSize().x / 2.0, window.getSize().y / 2.0);

	sf::RectangleShape water(sf::Vector2f(window.getSize().x, center.y));
	water.setPosition(0, center.y);
	water.setFillColor(getColorByMaterial(Water));

	Candle::Instance().SetSize(Config::Instance().CandleSize);
	CalculateCandleFloating(Candle::Instance(), window.getSize());
	
	sf::Clock clock;
    while(window.isOpen())
    {
	    const auto delta_time = clock.restart() * speedMultiplier;

		// Events handling
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed: window.close(); break;

                case sf::Event::Resized:
                    window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    gui.setView(window.getView());

					center = sf::Vector2f(window.getSize().x / 2.0, window.getSize().y / 2.0);
					water.setSize(sf::Vector2f(window.getSize().x, center.y));
					water.setPosition(0, center.y);
                break;

				case sf::Event::KeyPressed: 
					if(event.key.code == sf::Keyboard::R)
						Candle::Instance().Reset();
					if(event.key.code == sf::Keyboard::Space)
						Config::Instance().isPlaying = !Config::Instance().isPlaying;
				break;

                default: break;
            }

            gui.handleEvent(event);
        }
		
        // Update		
		CalculateCandleFloating(Candle::Instance(), window.getSize());
		if(Config::Instance().isPlaying)
			Candle::Instance().Update(delta_time);

		// Drawning
        window.clear(sf::Color::White);
		window.draw(water);
        window.draw(Candle::Instance());
        gui.draw();
        window.display();
    }

    return 0;
}
