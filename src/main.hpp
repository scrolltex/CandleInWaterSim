#pragma once
#include <SFML/System/Time.hpp>

extern sf::Time SimulationTime;

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

extern void ResetSimulation();
