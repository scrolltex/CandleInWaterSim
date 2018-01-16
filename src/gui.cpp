#include "gui.hpp"
#include <string>
#include "candle.hpp"

void CreateConfigMenu(tgui::Gui &gui);

void CreateGUI(tgui::Gui &gui)
{
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);

	// TODO: Global configs
	bool isPlaying = false;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top menu ///////////////////////////////////////////////////////////////////////////////////////////////////
	
	auto topPanel = tgui::Panel::create();
	topPanel->setSize(32 * 3 + 5 * 2 + 10, 32 + 10);
	topPanel->setPosition("(parent.width - width)/2", 0);
	gui.add(topPanel);

	auto playButton = tgui::Picture::create("resources/play.png");
	playButton->setSize(32, 32);
	playButton->setPosition(tgui::bindLeft(topPanel) + 5, tgui::bindTop(topPanel) + 5);
	playButton->setSmooth(true);
	gui.add(playButton);

	auto pauseButton = tgui::Picture::create("resources/pause.png");
	pauseButton->setSize(tgui::bindSize(playButton));
	pauseButton->setPosition(tgui::bindPosition(playButton));
	pauseButton->setSmooth(true);
	pauseButton->hide();
	pauseButton->disable();
	gui.add(pauseButton);

	auto resetButton = tgui::Picture::create("resources/reset.png");
	resetButton->setSize(32, 32);
	resetButton->setPosition(tgui::bindRight(playButton) + 5, tgui::bindTop(topPanel) + 5);
	resetButton->setSmooth(true);
	gui.add(resetButton);

	auto editButton = tgui::Picture::create("resources/edit.png");
	editButton->setSize(32, 32);
	editButton->setPosition(tgui::bindRight(resetButton) + 5, tgui::bindTop(topPanel) + 5);
	editButton->setSmooth(true);
	gui.add(editButton);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Register callbacks /////////////////////////////////////////////////////////////////////////////////////////
		
	auto setPlaying = [&isPlaying, playButton, pauseButton] (bool playing)
	{
		isPlaying = playing;

		if(isPlaying)
		{
			playButton->hide();
			playButton->disable();
			pauseButton->show();
			pauseButton->enable();
		}
		else
		{
			playButton->show();
			playButton->enable();
			pauseButton->hide();
			pauseButton->disable();
		}
	};

	playButton->connect("clicked", setPlaying, true);
	pauseButton->connect("clicked", setPlaying, false);

	resetButton->connect("clicked", [] () {
		Candle::Instance().Reset();
	});

	editButton->connect("clicked", [&gui] () {
		if(!gui.get("ConfigWindow"))
			CreateConfigMenu(gui);
	});
}

void CreateConfigMenu(tgui::Gui &gui)
{
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Config menu ////////////////////////////////////////////////////////////////////////////////////////////////

	auto configWindow = tgui::ChildWindow::create();
    configWindow->setSize(wnd_width * 0.75, wnd_height * 0.55);
    configWindow->setPosition((tgui::bindSize(gui) - tgui::bindSize(configWindow)) / 2);
    configWindow->setTitle("Configs");
    gui.add(configWindow, "ConfigWindow");

	auto grid = tgui::Grid::create();
	configWindow->add(grid);

	auto materialNameHeader = tgui::Label::create("Material");
	grid->addWidget(materialNameHeader, 0, 0);
	
	auto materialDensityHeader = tgui::Label::create("Density");
	grid->addWidget(materialDensityHeader, 0, 1);
	
	auto materialHeatCapacityHeader = tgui::Label::create("Heat capacity");
	grid->addWidget(materialHeatCapacityHeader, 0, 2);
	
	auto materialThermalConductivityHeader = tgui::Label::create("Thermal conductivity");
	grid->addWidget(materialThermalConductivityHeader, 0, 3);

    auto createForMaterial = [&] (std::string matName, int vertical) {	    		
		auto materialName = tgui::Label::create(matName);
		grid->addWidget(materialName, vertical, 0);

		auto materialDensity = tgui::EditBox::create();
		materialDensity->setSize(materialDensityHeader->getSize().x, 25);
		materialDensity->setInputValidator(tgui::EditBox::Validator::Float);
		//materialDensity->setText(std::to_string(speedMultiplier));
		grid->addWidget(materialDensity, vertical, 1);

		auto materialHeatCapacity = tgui::EditBox::copy(materialDensity);
		materialHeatCapacity->setSize(materialHeatCapacityHeader->getSize().x, 25);
		//materialHeatCapacity->setText(std::to_string(speedMultiplier));
		grid->addWidget(materialHeatCapacity, vertical, 2);

		auto materialThermalConductivity = tgui::EditBox::copy(materialDensity);
		materialThermalConductivity->setSize(materialThermalConductivityHeader->getSize().x, 25);
		//materialThermalConductivity->setText(std::to_string(speedMultiplier));
		grid->addWidget(materialThermalConductivity, vertical, 3);
    };

	/*
		Air,      
		Water,
		Iron,
		Paraffin,
		ParaffinLiquid,
		Fire
	 */

	createForMaterial("Air", 1);
	createForMaterial("Water", 2);
	createForMaterial("Iron", 3);
	createForMaterial("Paraffin", 4);
	createForMaterial("ParaffinLiquid", 5);
	grid->setSize(configWindow->getSize().x, "100%");
	
	auto splitPanel = tgui::Panel::create();
	splitPanel->setSize(grid->getSize().x - 10, 1);
	splitPanel->setPosition(tgui::bindLeft(grid) + 5, tgui::bindBottom(grid) + 1);
	splitPanel->setBackgroundColor(sf::Color::Black);
	configWindow->add(splitPanel);
	
	auto candleSizeLabel = tgui::Label::create("Size: ");
	candleSizeLabel->setAutoSize(false);
	candleSizeLabel->setSize(125, 25);
	candleSizeLabel->setPosition(tgui::bindLeft(grid) + 5, tgui::bindBottom(splitPanel) + 3);
	candleSizeLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	configWindow->add(candleSizeLabel);

	auto candleSizeLayout = tgui::HorizontalLayout::create();
	candleSizeLayout->setSize(125, 25);
	candleSizeLayout->setPosition(tgui::bindRight(candleSizeLabel) + 5, tgui::bindTop(candleSizeLabel));
	configWindow->add(candleSizeLayout);

    auto candleWidthEdit = tgui::EditBox::create();
    candleWidthEdit->setInputValidator("[0-9]{1,3}");
	candleWidthEdit->setAlignment(tgui::EditBox::Alignment::Right);
    //candleWidthEdit->setText(std::to_string(speedMultiplier));
    candleSizeLayout->add(candleWidthEdit);
	
	auto label = tgui::Label::create("x");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	candleSizeLayout->add(label);
	candleSizeLayout->setFixedSize(label, static_cast<float>(label->getTextSize()));

    auto candleHeightEdit = tgui::EditBox::create();
    candleHeightEdit->setInputValidator("[0-9]{1,3}");
	candleHeightEdit->setAlignment(tgui::EditBox::Alignment::Left);
    //candleHeightEdit->setText(std::to_string(speedMultiplier));
    candleSizeLayout->add(candleHeightEdit);

    auto applyButton = tgui::Button::create();
    applyButton->setSize(125, 25);
    applyButton->setPosition(configWindow->getSize().x - applyButton->getSize().x - 5, configWindow->getSize().y - applyButton->getSize().y - 5);
    applyButton->setText("Apply");
    configWindow->add(applyButton);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Register callbacks /////////////////////////////////////////////////////////////////////////////////////////

	// TODO: Applying new configs

}
