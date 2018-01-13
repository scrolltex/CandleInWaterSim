#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <iostream>
#include <string>
#include "candle.hpp"

bool isPlaying = false;
float speedMultiplier = 900;

void createGUI(tgui::Gui &gui)
{
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);

    tgui::Button::Ptr runButton = tgui::Button::create();
    runButton->setSize(100, 25);
    runButton->setPosition(wnd_width - runButton->getSize().x - 15, 15);
    runButton->setText("Play");
    gui.add(runButton);

    runButton->connect("pressed", [runButton] () {
        isPlaying = !isPlaying;
        runButton->setText(isPlaying ? "Pause" : "Play");
    });
	
    tgui::EditBox::Ptr speedMultiplierEdit = tgui::EditBox::create();
    speedMultiplierEdit->setSize(100, 25);
    speedMultiplierEdit->setPosition(wnd_width - runButton->getSize().x - 15, 50);
    speedMultiplierEdit->setInputValidator(tgui::EditBox::Validator::Float);
    speedMultiplierEdit->setText(std::to_string(speedMultiplier));
    gui.add(speedMultiplierEdit);

    tgui::Button::Ptr applyButton = tgui::Button::create();
    applyButton->setSize(100, 25);
    applyButton->setPosition(wnd_width - applyButton->getSize().x - 15, 85);
    applyButton->setText("Apply");
    gui.add(applyButton);

    applyButton->connect("pressed", [speedMultiplierEdit] () {
        speedMultiplier = std::stof(speedMultiplierEdit->getText().toAnsiString());
    });
}

void CalculateCandleFloating(Candle &candle, sf::Vector2u wnd_size)
{
	const auto center = sf::Vector2f(wnd_size.x / 2.0, wnd_size.y / 2.0);
	const auto candle_offset = (candle.CalculateAverageDensity() / getDensity(Water)) * candle.GetSizeInPx().y;
	candle.SetWaterLevel(candle_offset);
	candle.setPosition(center.x, std::min(center.y + candle_offset, static_cast<double>(wnd_size.y)));
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Candle in water simulation", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(30);
    tgui::Gui gui(window);

	sf::Clock clock;
	const auto center = sf::Vector2f(window.getSize().x / 2.0, window.getSize().y / 2.0);

	sf::RectangleShape water(sf::Vector2f(window.getSize().x, center.y));
	water.setPosition(0, center.y);
	water.setFillColor(getColorByMaterial(Water));

	Candle candle;
	CalculateCandleFloating(candle, window.getSize());
	
    try
    {
        createGUI(gui);
    }
    catch(const tgui::Exception &e)
    {
        std::cerr << "Failed to create TGUI widgets: " << e.what() << std::endl;
        return 1;
    }

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
                break;

				case sf::Event::KeyPressed: 
					if(event.key.code == sf::Keyboard::R)
						candle.Reset();
				break;

                default: break;
            }

            gui.handleEvent(event);
        }
		

        // Update
		if(isPlaying)
		{
			CalculateCandleFloating(candle, window.getSize());
			candle.Update(delta_time);
		}

		// Drawning
        window.clear(sf::Color::White);
		window.draw(water);
        window.draw(candle);
        gui.draw();
        window.display();
    }

    return 0;
}
