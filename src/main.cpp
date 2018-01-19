#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include "candle.hpp"
#include "gui.hpp"
#include "config.hpp"

#include "gif.h"

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
	
	// For recording to gif
	bool isRecording = false;
	GifWriter gif_writer;

	sf::Clock clock;
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
					else if(event.key.code == sf::Keyboard::Space)
						Config::Instance().isPlaying = !Config::Instance().isPlaying;
					else if(event.key.code == sf::Keyboard::H)
						Config::Instance().drawHeatmap = !Config::Instance().drawHeatmap;
					else if(event.key.code == sf::Keyboard::P)
					{
						if(!isRecording)
							GifBegin(&gif_writer, "CandleInWaterSim.gif", window.getSize().x, window.getSize().y, 1);
						else
							GifEnd(&gif_writer);

						isRecording = !isRecording;						
					}
				break;

                default: break;
            }

            gui.handleEvent(event);
        }

        // Update	
    	static double __physics_time = 0;
		__physics_time += delta_time.asSeconds();
		if(__physics_time >= 0.03)
		{
			CalculateCandleFloating(Candle::Instance(), window.getSize());
			if(Config::Instance().isPlaying)
				Candle::Instance().Update(sf::seconds(__physics_time) * speedMultiplier);
			__physics_time = 0;
		}

		// Drawning
        window.clear(sf::Color::White);
		window.draw(water);
        window.draw(Candle::Instance());
        gui.draw();
        window.display();

		// Recording to gif
		if(isRecording)
		{
			//TODO: Optimization very needed
			static double __time = 0;
			__time += delta_time.asMilliseconds();
			if(__time >= 250)
			{
				sf::Texture texture;
				texture.create(window.getSize().x, window.getSize().y);			
				texture.update(window);
				GifWriteFrame(&gif_writer, texture.copyToImage().getPixelsPtr(), texture.getSize().x, texture.getSize().y, __time / 10);

				__time = 0;
			}
		}
    }

    return 0;
}
