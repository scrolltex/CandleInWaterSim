#include "materials.hpp"
#include <map>

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

double getDensityByMaterial(MaterialType mat)
{
	std::map<MaterialType, double> arr = {
		{ Air, 0.001225 },
		{ Water, 1 },
		{ Iron, 7.9 },
		{ Paraffin, 0.915 },
		{ ParaffinLiquid, 0.915 },
		{ Fire, 0 },
	};

	return arr.at(mat);
}
