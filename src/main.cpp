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

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Candle in water simulation", sf::Style::Titlebar | sf::Style::Close);
    tgui::Gui gui(window);

	sf::Clock clock;
	Candle candle;

	const auto center = sf::Vector2f(window.getSize().x / 2.0, window.getSize().y / 2.0);

	sf::RectangleShape water(sf::Vector2f(window.getSize().x, center.y));
	water.setPosition(0, center.y);
	water.setFillColor(getColorByMaterial(Water));

	candle.setPosition(center.x, center.y + candle.GetSizeInPx().y * units::pixelsPerUnit);
	
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

                default: break;
            }

            gui.handleEvent(event);
        }

        // Candle floating
        const auto candle_offset = (candle.CalculateAverageDensity() / getDensity(Water)) * candle.GetSizeInPx().y;
        candle.SetWaterLevel(candle_offset);
        candle.setPosition(center.x, std::min(center.y + candle_offset, static_cast<double>(window.getSize().y)));

        // Update
        if(isPlaying)
            candle.Update(delta_time);

		// Drawning
        window.clear(sf::Color::White);
		window.draw(water);
        window.draw(candle);
        gui.draw();
        window.display();
    }

    return 0;
}
