#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum MaterialType {
    Air,      
    Water,
    Iron,
    Paraffin,
    ParaffinLiquid,
    Fire
};

#define STRINGIFY(VAR) #VAR

std::string MaterialTypeToString(MaterialType mat);

struct MaterialVars {
    MaterialVars() : MaterialVars(0, 0, 0)
    {

    }

    MaterialVars(double density, double heatCapacity, double thermalConductivity)
    {
        this->density = density;
        this->heatCapacity = heatCapacity;
        this->thermalConductivity = thermalConductivity;
    }

    double density;
    double heatCapacity;
    double thermalConductivity;
};

sf::Color getColorByMaterial(MaterialType mat);
