#pragma once
#include <atomic>
#include <SFML/System/Time.hpp>

extern sf::Time SimulationTime;
extern std::atomic<sf::Time> DeltaTime;

extern void ResetSimulation();
