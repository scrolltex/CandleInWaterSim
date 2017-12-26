#include "materials.hpp"
#include <map>
#include <cmath>

sf::Color getColorByMaterial(MaterialType mat)
{
    std::map<MaterialType, sf::Color> colors = {
        { Air,            sf::Color(255, 255, 255, 0) },
        { Water,          sf::Color(135, 206, 235, 200) },
        { Iron,           sf::Color(105, 105, 105) },
        { Paraffin,       sf::Color(238, 232, 170) },
        { ParaffinLiquid, sf::Color(238, 232, 170, 100) },
        { Fire,           sf::Color(255, 0, 0) },
    };

    return colors.at(mat);
}

double getDensity(MaterialType mat)
{
	std::map<MaterialType, double> arr = {
		{ Air, 1.265 },
		{ Water, 1000 },
		{ Iron, 3.8 * pow(10, 3) },
		{ Paraffin, 915 },
		{ ParaffinLiquid, 915 },
		{ Fire, 0 },
	};

	return arr.at(mat);
}

double getHeatCapacity(MaterialType mat)
{
	std::map<MaterialType, double> arr = {
		{ Air, 1000 },
		{ Water, 4183 },
		{ Iron, 640.57 },
		{ Paraffin, 3220 },
		{ ParaffinLiquid, 3220 }
	};

	return arr.at(mat);
}

double getThermalConductivity(MaterialType mat)
{
	std::map<MaterialType, double> arr = {
		{ Air, 0.022 },
		{ Water, 0.6 },
		{ Iron, 92 },
		{ Paraffin, 0.26 },
		{ ParaffinLiquid, 0.26 }
	};

	return arr.at(mat);
}
