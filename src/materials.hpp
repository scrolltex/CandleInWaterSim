#pragma once
#include <SFML/Graphics.hpp>

enum MaterialType {
    Air,      
    Water,
    Iron,
    Paraffin,
    ParaffinLiquid,
    Fire
};

sf::Color getColorByMaterial(MaterialType mat);
double getDensity(MaterialType mat);
double getHeatCapacity(MaterialType mat);
double getThermalConductivity(MaterialType mat);
