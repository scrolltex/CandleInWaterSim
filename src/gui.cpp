#include "gui.hpp"

#include <string>
#include <map>

#include "main.hpp"
#include "config.hpp"
#include "candle.hpp"

// Callbacks
std::function<void()> gui::onApplyCallback;

void setPlaying(tgui::Gui& gui, bool playing)
{
	static bool _playing_cached = Config::Instance().isPlaying;
	if(_playing_cached == playing) return;
	_playing_cached = playing;
	
	static auto playButton = gui.get<tgui::Picture>("PlayButton");
	static auto pauseButton = gui.get<tgui::Picture>("PauseButton");

	if(!playButton || !pauseButton)
		return;

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
}

void gui::ShowMessage(tgui::Gui &gui, std::string message, std::string title)
{
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);

	auto messageBox = tgui::MessageBox::create();
	messageBox->setTitle(title);
	messageBox->setText(message);
	messageBox->setPosition(wnd_width/2 - tgui::bindWidth(messageBox)/2, 
							wnd_height/2 - tgui::bindHeight(messageBox)/2);
	gui.add(messageBox);	
}

inline void CreateHeatmapBar(tgui::Gui &gui);

inline void CreateConfigMenu(tgui::Gui &gui);

void gui::CreateGUI(sf::RenderWindow &window, tgui::Gui &gui)
{
	auto theme = tgui::Theme::create("resources/theme.txt");
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Top menu ///////////////////////////////////////////////////////////////////////////////////////////////////
	
	const uint8_t icon_size = 32;
	const uint8_t icon_count = 4;

	auto topPanel = tgui::Panel::create();
	topPanel->setSize(icon_size * icon_count + 5 * (icon_count-1) + 10, icon_size + 10);
	topPanel->setPosition("(parent.width - width)/2", 0);
	gui.add(topPanel);

	auto playButton = tgui::Picture::create("resources/play.png");
	playButton->setSize(icon_size, icon_size);
	playButton->setPosition(tgui::bindLeft(topPanel) + 5, tgui::bindTop(topPanel) + 5);
	playButton->setSmooth(true);
	gui.add(playButton, "PlayButton");
	
	auto pauseButton = tgui::Picture::create("resources/pause.png");
	pauseButton->setSize(tgui::bindSize(playButton));
	pauseButton->setPosition(tgui::bindPosition(playButton));
	pauseButton->setSmooth(true);
	pauseButton->hide();
	pauseButton->disable();
	gui.add(pauseButton, "PauseButton");

	auto resetButton = tgui::Picture::create("resources/reset.png");
	resetButton->setSize(icon_size, icon_size);
	resetButton->setPosition(tgui::bindRight(playButton) + 5, tgui::bindTop(topPanel) + 5);
	resetButton->setSmooth(true);
	gui.add(resetButton);

	auto heatmapButton = tgui::Picture::create("resources/heatmap.png");
	heatmapButton->setSize(icon_size, icon_size);
	heatmapButton->setPosition(tgui::bindRight(resetButton) + 5, tgui::bindTop(topPanel) + 5);
	heatmapButton->setSmooth(true);
	gui.add(heatmapButton);

	auto editButton = tgui::Picture::create("resources/edit.png");
	editButton->setSize(icon_size, icon_size);
	editButton->setPosition(tgui::bindRight(heatmapButton) + 5, tgui::bindTop(topPanel) + 5);
	editButton->setSmooth(true);
	gui.add(editButton);

	auto simTimeLabel = tgui::Label::create("00:00:00");
	simTimeLabel->setSize(tgui::bindWidth(gui)/2, 25);
	simTimeLabel->setPosition(tgui::bindWidth(gui)/2 - tgui::bindWidth(simTimeLabel)/2, tgui::bindBottom(topPanel) + 5);
	simTimeLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	gui.add(simTimeLabel, "SimulationTimeLabel");
		
	CreateHeatmapBar(gui);

	// Point info tooltip
	tgui::Label::Ptr pointInfoTooltip = theme->load("Tooltip");
	pointInfoTooltip->hide();
	gui.add(pointInfoTooltip, "PointInfoTooltip");
	tgui::Panel::Ptr hoveredPoint = tgui::Panel::create({units::pixelsPerUnit, units::pixelsPerUnit});
	hoveredPoint->hide();
	gui.add(hoveredPoint, "HoveredPoint");
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Register callbacks /////////////////////////////////////////////////////////////////////////////////////////
	playButton->connect("clicked", setPlaying, std::ref(gui), true);
	pauseButton->connect("clicked", setPlaying, std::ref(gui), false);

	resetButton->connect("clicked", [] () {
		ResetSimulation();
	});

	heatmapButton->connect("clicked", [] () {
		Config::Instance().drawHeatmap = !Config::Instance().drawHeatmap;
	});

	editButton->connect("clicked", [&gui] () {
		if(!gui.get("ConfigWindow"))
			CreateConfigMenu(gui);
	});
}

inline void CreateHeatmapBar(tgui::Gui &gui)
{
    auto wnd_width = tgui::bindWidth(gui);
    auto wnd_height = tgui::bindHeight(gui);
	
	const sf::Vector2i bar_size = {25, 200};
	sf::Image heatmap_bar_img;
	heatmap_bar_img.create(bar_size.x, bar_size.y);

	for(auto y = 0; y < bar_size.y; ++y)
	{
		const double mix = static_cast<double>(y) / bar_size.y;
		for(auto x = 0; x < bar_size.x; ++x)
			heatmap_bar_img.setPixel(x, y, getHeatMapColor(1-mix));
	}

	static sf::Texture texture;
	texture.loadFromImage(heatmap_bar_img);
	auto heatmapBar = tgui::Picture::create(texture);
	heatmapBar->setSize(bar_size.x, bar_size.y);
	heatmapBar->setPosition(wnd_width - tgui::bindWidth(heatmapBar), wnd_height/2 - tgui::bindHeight(heatmapBar)/2);
	heatmapBar->hide();
	gui.add(heatmapBar, "HeatmapBar");

	auto maxTempLabel = tgui::Label::create("600");
	maxTempLabel->setAutoSize(true);
	maxTempLabel->setPosition(tgui::bindLeft(heatmapBar) - tgui::bindWidth(maxTempLabel) - 5, tgui::bindTop(heatmapBar));
	maxTempLabel->hide();
	gui.add(maxTempLabel, "HeatmapBar_MaxT");

	auto minTempLabel = tgui::Label::create("25");
	minTempLabel->setAutoSize(true);
	minTempLabel->setPosition(tgui::bindLeft(heatmapBar) - tgui::bindWidth(minTempLabel) - 5, tgui::bindBottom(heatmapBar) - tgui::bindHeight(minTempLabel));
	minTempLabel->hide();
	gui.add(minTempLabel, "HeatmapBar_MinT");
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

inline void CreateConfigMenu(tgui::Gui &gui)
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
	
	// � little magic for beautiful code)
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

inline void UpdatePointInfoTooltip(sf::RenderWindow &window, tgui::Gui &gui)
{
	static auto wnd = gui.getWindow();
	assert(wnd != nullptr);

	static auto tooltip = gui.get<tgui::Label>("PointInfoTooltip");
	assert(tooltip != nullptr);

	static auto hoveredPoint = gui.get("HoveredPoint");
	assert(hoveredPoint != nullptr);

	const auto candle_pos = Candle::Instance().getPosition();
	const auto candle_size = static_cast<sf::Vector2f>(Candle::Instance().GetSizeInPx());

	sf::FloatRect rect = {
		candle_pos.x - candle_size.x/2.0f,
		candle_pos.y - candle_size.y,
		static_cast<float>(candle_size.x),
		static_cast<float>(candle_size.y)
	};

	const auto mouse_pos = sf::Mouse::getPosition(window);

	if(rect.contains(static_cast<sf::Vector2f>(mouse_pos)))
	{
		auto point_pos = sf::Vector2i(std::floor((mouse_pos.x - candle_pos.x + candle_size.x/2) / units::pixelsPerUnit), 
									  std::floor((mouse_pos.y - candle_pos.y + candle_size.y) / units::pixelsPerUnit));

		point_pos.y += 9;

		auto point = Candle::Instance().GetPoint(point_pos);
		
		tooltip->setText(MaterialTypeToString(point.GetMaterial()) + "\n" + std::to_string(point.temperature));
		tooltip->setPosition(mouse_pos.x + 8, mouse_pos.y + 0);
		
		hoveredPoint->setPosition(rect.left + point_pos.x * units::pixelsPerUnit, 
								  rect.top + (point_pos.y - 9) * units::pixelsPerUnit);

		if(!tooltip->isVisible())
		{
			tooltip->show();
			hoveredPoint->show();
		}
	}
	else
	{
		if(tooltip->isVisible())
		{
			tooltip->hide();
			hoveredPoint->hide();
		}
	}
}

void gui::UpdateGUI(sf::RenderWindow &window, tgui::Gui &gui)
{
	static auto simTimeLabel = gui.get<tgui::Label>("SimulationTimeLabel");
	if(simTimeLabel)
		simTimeLabel->setText(FormatTime(SimulationTime));
	
	static auto heatmapBar = gui.get<tgui::Picture>("HeatmapBar");
	static auto heatmapBar_maxT = gui.get<tgui::Label>("HeatmapBar_MaxT");
	static auto heatmapBar_minT = gui.get<tgui::Label>("HeatmapBar_MinT");

	if(heatmapBar && heatmapBar_maxT && heatmapBar_minT)
	{
		static bool _drawHeatmap_cached = Config::Instance().drawHeatmap;
		if(_drawHeatmap_cached != Config::Instance().drawHeatmap)
		{
			_drawHeatmap_cached = Config::Instance().drawHeatmap;
			if(_drawHeatmap_cached)
			{
				heatmapBar->show();
				heatmapBar_maxT->show();
				heatmapBar_minT->show();
			}
			else
			{
				heatmapBar->hide();
				heatmapBar_maxT->hide();
				heatmapBar_minT->hide();
			}
		}
	}	

	setPlaying(gui, Config::Instance().isPlaying);

	UpdatePointInfoTooltip(window, gui);
}
