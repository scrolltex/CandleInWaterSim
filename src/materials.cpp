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
