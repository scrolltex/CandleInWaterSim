#include "gui.hpp"
#include <string>
#include <map>
#include "candle.hpp"
#include "config.hpp"

// Callbacks
std::function<void()> gui::onApplyCallback;

void CreateConfigMenu(tgui::Gui &gui);

void gui::CreateGUI(tgui::Gui &gui)
{
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top menu ///////////////////////////////////////////////////////////////////////////////////////////////////
	
	const int icon_size = 32;

	auto topPanel = tgui::Panel::create();
	topPanel->setSize(icon_size * 3 + 5 * 2 + 10, icon_size + 10);
	topPanel->setPosition("(parent.width - width)/2", 0);
	gui.add(topPanel);

	auto playButton = tgui::Picture::create("resources/play.png");
	playButton->setSize(icon_size, icon_size);
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
	resetButton->setSize(icon_size, icon_size);
	resetButton->setPosition(tgui::bindRight(playButton) + 5, tgui::bindTop(topPanel) + 5);
	resetButton->setSmooth(true);
	gui.add(resetButton);

	auto editButton = tgui::Picture::create("resources/edit.png");
	editButton->setSize(icon_size, icon_size);
	editButton->setPosition(tgui::bindRight(resetButton) + 5, tgui::bindTop(topPanel) + 5);
	editButton->setSmooth(true);
	gui.add(editButton);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Register callbacks /////////////////////////////////////////////////////////////////////////////////////////
		
	auto setPlaying = [playButton, pauseButton] (bool playing)
	{
		Config::Instance().isPlaying = playing;
		if(Config::Instance().isPlaying)
		{
			playButton->disable();
			playButton->hide();
			pauseButton->show();
			pauseButton->enable();
		}
		else
		{
			pauseButton->disable();
			pauseButton->hide();
			playButton->show();
			playButton->enable();
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

struct MaterialEditBox
{
	MaterialEditBox(tgui::EditBox::Ptr Density, tgui::EditBox::Ptr HeatCapacity, tgui::EditBox::Ptr ThermalConductivity)
	{
		this->Density = Density;
		this->HeatCapacity = HeatCapacity;
		this->ThermalConductivity = ThermalConductivity;
	}

	tgui::EditBox::Ptr Density;
	tgui::EditBox::Ptr HeatCapacity;
	tgui::EditBox::Ptr ThermalConductivity;
};

std::map<MaterialType, MaterialEditBox> MaterialEditBoxes;

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
	grid->setSize(tgui::bindWidth(configWindow), "100%");
	configWindow->add(grid);

	auto materialNameHeader = tgui::Label::create("Material");
	materialNameHeader->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	materialNameHeader->setSize(175, 25);
	grid->addWidget(materialNameHeader, 0, 0);
	
	// Calculate once the size of EditBox cell
	const tgui::Layout2d columnSize = { (tgui::bindWidth(grid) - tgui::bindWidth(materialNameHeader) - 10 * 4) / 3.0, 25 };

	auto materialDensityHeader = tgui::Label::create("Density");
	materialDensityHeader->setSize(columnSize);
	materialDensityHeader->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	grid->addWidget(materialDensityHeader, 0, 1);
	
	auto materialHeatCapacityHeader = tgui::Label::create("Heat cap.");
	materialHeatCapacityHeader->setSize(columnSize);
	materialHeatCapacityHeader->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	grid->addWidget(materialHeatCapacityHeader, 0, 2);
	
	auto materialThermalConductivityHeader = tgui::Label::create("Thermal cond.");
	materialThermalConductivityHeader->setSize(columnSize);
	materialThermalConductivityHeader->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	grid->addWidget(materialThermalConductivityHeader, 0, 3);
	
	// À little magic for beautiful code)
	MaterialEditBoxes.clear();
	int _current_row = 1;
    auto createForMaterial = [&] (MaterialType mat) {	    
		auto mat_vars = Config::Instance().Materials.at(mat);

		auto materialName = tgui::Label::create(MaterialTypeToString(mat));
		materialName->setSize(tgui::bindSize(materialNameHeader));
		materialName->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		grid->addWidget(materialName, _current_row, 0);

		auto materialDensity = tgui::EditBox::create();
		materialDensity->setSize(columnSize);
		materialDensity->setInputValidator(tgui::EditBox::Validator::Float);
		materialDensity->setText(std::to_string(mat_vars.density));
		grid->addWidget(materialDensity, _current_row, 1);

		auto materialHeatCapacity = tgui::EditBox::copy(materialDensity);
		materialHeatCapacity->setText(std::to_string(mat_vars.heatCapacity));
		grid->addWidget(materialHeatCapacity, _current_row, 2);

		auto materialThermalConductivity = tgui::EditBox::copy(materialDensity);
		materialThermalConductivity->setText(std::to_string(mat_vars.thermalConductivity));
		grid->addWidget(materialThermalConductivity, _current_row, 3);

		++_current_row;
		MaterialEditBoxes.emplace(mat, MaterialEditBox(materialDensity, materialHeatCapacity, materialThermalConductivity));
    };

	// Creating rows for materials
	createForMaterial(Air);
	createForMaterial(Water);
	createForMaterial(Iron);
	createForMaterial(Paraffin);
	createForMaterial(ParaffinLiquid);
		
	// Splitter
	auto splitPanel = tgui::Panel::create();
	splitPanel->setSize(tgui::bindWidth(grid) - 10, 1);
	splitPanel->setPosition(tgui::bindLeft(grid) + 5, tgui::bindBottom(grid) + 1);
	splitPanel->setBackgroundColor(sf::Color::Black);
	configWindow->add(splitPanel);
	
	// Candle size
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
    candleWidthEdit->setText(std::to_string(Config::Instance().CandleSize.x));
    candleSizeLayout->add(candleWidthEdit);
	
	auto label = tgui::Label::create("x");
	label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	candleSizeLayout->add(label);
	candleSizeLayout->setFixedSize(label, static_cast<float>(label->getTextSize()));

    auto candleHeightEdit = tgui::EditBox::create();
    candleHeightEdit->setInputValidator("[0-9]{1,3}");
	candleHeightEdit->setAlignment(tgui::EditBox::Alignment::Left);
    candleHeightEdit->setText(std::to_string(Config::Instance().CandleSize.y));
    candleSizeLayout->add(candleHeightEdit);

    auto applyButton = tgui::Button::create();
    applyButton->setSize(125, 25);
    applyButton->setPosition(configWindow->getSize().x - applyButton->getSize().x - 5, configWindow->getSize().y - applyButton->getSize().y - 5);
    applyButton->setText("Apply");
    configWindow->add(applyButton);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Register callbacks /////////////////////////////////////////////////////////////////////////////////////////

	applyButton->connect("clicked", [=] () {
		// Apply material vars
		for(auto &eb : MaterialEditBoxes)
		{
			MaterialVars matVars;

			matVars.density				= std::stod(eb.second.Density->getText().toAnsiString());
			matVars.heatCapacity		= std::stod(eb.second.HeatCapacity->getText().toAnsiString());
			matVars.thermalConductivity = std::stod(eb.second.ThermalConductivity->getText().toAnsiString());

			Config::Instance().Materials.at(eb.first) = matVars;
		}

		// Apply candle size
		Config::Instance().CandleSize = { std::stoi(candleWidthEdit->getText().toAnsiString()), 
										  std::stoi(candleHeightEdit->getText().toAnsiString()) };

		// Invoke callback and close window
		gui::onApplyCallback();
		configWindow->destroy();
	});
}
