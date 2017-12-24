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
double getDensityByMaterial(MaterialType mat);
