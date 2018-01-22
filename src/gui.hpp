#pragma once
#include <TGUI/TGUI.hpp>
#include <functional>

namespace gui{
    void CreateGUI(tgui::Gui &gui);
	void UpdateGUI(tgui::Gui &gui);
	
    extern std::function<void()> onApplyCallback;

	inline std::string FormatTime(sf::Time time)
	{
		int secs = time.asSeconds();
		const int hours = (secs / 60 / 60) % 24;
		const int minutes = (secs / 60) % 60;
		const int seconds = secs % 60;

		std::string time_str;
		time_str += (hours < 10 ? "0" : "") + std::to_string(hours);
		time_str += ":";
		time_str += (minutes < 10 ? "0" : "") + std::to_string(minutes);
		time_str += ":";
		time_str += (seconds < 10 ? "0" : "") + std::to_string(seconds);

		return time_str;
	}
}
