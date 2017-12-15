#pragma once
#include <SFML/Graphics.hpp>
#include <map>

enum MaterialType {
    Air,      
    Water,
    Iron,
    Paraffin,
    ParaffinLiquid,
    Fire
};

sf::Color getColorByMaterial(MaterialType mat);
