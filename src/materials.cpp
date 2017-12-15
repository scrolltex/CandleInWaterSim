#include "materials.hpp"

sf::Color getColorByMaterial(MaterialType mat)
{
    std::map<MaterialType, sf::Color> colors = {
        { MaterialType::Air,            sf::Color(255, 255, 255, 0) },
        { MaterialType::Water,          sf::Color(135, 206, 235, 200) },
        { MaterialType::Iron,           sf::Color(105, 105, 105) },
        { MaterialType::Paraffin,       sf::Color(238, 232, 170) },
        { MaterialType::ParaffinLiquid, sf::Color(238, 232, 170, 100) },
        { MaterialType::Fire,           sf::Color(255, 0, 0) },
    };

    return colors[mat];
}
