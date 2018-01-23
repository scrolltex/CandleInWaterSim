#include <iostream>

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include "gif.h"

#include "main.hpp"
#include "config.hpp"
#include "candle.hpp"
#include "gui.hpp"

sf::Time SimulationTime = sf::Time::Zero;
float speedMultiplier = 900;
	
// For recording to gif
bool isRecording = false;
GifWriter gif_writer;

void ResetSimulation()
{
	Config::Instance().isPlaying = false;
	Candle::Instance().Reset();
	SimulationTime = sf::Time::Zero;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Candle in water simulation"/*, sf::Style::Titlebar | sf::Style::Close*/);
	window.setFramerateLimit(60);
    tgui::Gui gui(window);

	// Load font
	sf::Font font;
	font.loadFromFile("resources/DejaVuSans.ttf");
	gui.setFont(font);

    try
    {
        gui::CreateGUI(window, gui);
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

	// Candle setup
	Candle::Instance().SetSize(Config::Instance().CandleSize);
	Candle::Instance().CalculateFloating(window.getSize());
	Candle::Instance().onCandleExtinguished = [&] {
		Config::Instance().isPlaying = false;

		if(isRecording)
		{
			GifEnd(&gif_writer);
			gui::ShowMessage(gui, "GIF saved!");
		}
	};

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
						ResetSimulation();
					else if(event.key.code == sf::Keyboard::Space)
						Config::Instance().isPlaying = !Config::Instance().isPlaying;
					else if(event.key.code == sf::Keyboard::H)
						Config::Instance().drawHeatmap = !Config::Instance().drawHeatmap;
					else if(event.key.code == sf::Keyboard::P)
					{
						if(!isRecording)
							GifBegin(&gif_writer, "CandleInWaterSim.gif", window.getSize().x, window.getSize().y, 1);
						else
						{
							GifEnd(&gif_writer);
							gui::ShowMessage(gui, "GIF saved!");
						}

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
			if(Config::Instance().isPlaying)
			{
				SimulationTime += sf::seconds(__physics_time) * speedMultiplier;

				Candle::Instance().Update(sf::seconds(__physics_time) * speedMultiplier);
				Candle::Instance().CalculateFloating(window.getSize());
			}

			__physics_time = 0;
		}

		gui::UpdateGUI(window, gui);

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
