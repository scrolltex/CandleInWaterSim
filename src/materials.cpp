#include "materials.hpp"
#include <map>
#include <cmath>

std::string MaterialTypeToString(MaterialType mat)
{
    std::vector<std::string> strs = {
        STRINGIFY(Air),
        STRINGIFY(Water),
        STRINGIFY(Iron),
        STRINGIFY(Paraffin),
        STRINGIFY(ParaffinLiquid),
        STRINGIFY(Fire)
    };

	return strs.at((int)mat);
}

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

sf::Color ColorMix(sf::Color c1, sf::Color c2, double mix)
{
	sf::Color result;
	result.r = c1.r * (1 - mix) + c2.r * mix;
	result.g = c1.g * (1 - mix) + c2.g * mix;
	result.b = c1.b * (1 - mix) + c2.b * mix;
	result.a = c1.a * (1 - mix) + c2.a * mix;
	return result;
}
