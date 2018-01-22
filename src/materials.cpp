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

sf::Color getHeatMapColor(double value)
{
	const int NUM_COLORS = 4;
	static const float color[NUM_COLORS][3] = { {0,0,1}, {0,1,0}, {1,1,0}, {1,0,0} };
	// A static array of 4 colors:  (blue,   green,  yellow,  red) using {r,g,b} for each.
 
	int idx1;        // |-- Our desired color will be between these two indexes in "color".
	int idx2;        // |
	float fractBetween = 0;  // Fraction between "idx1" and "idx2" where our value is.
     
	if(value <= 0) // accounts for an input <=0
	{
		idx1 = idx2 = 0;
	}
	else if(value >= 1) // accounts for an input >=0
	{
		idx1 = idx2 = NUM_COLORS-1;
	}
	else
	{
		value = value * (NUM_COLORS-1);        // Will multiply value by 3.
		idx1  = floor(value);                  // Our desired color will be after this index.
		idx2  = idx1+1;                        // ... and before this index (inclusive).
		fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).
	}
 
	sf::Color result;
	result.r = ((color[idx2][0] - color[idx1][0])*fractBetween + color[idx1][0])*255;
	result.g = ((color[idx2][1] - color[idx1][1])*fractBetween + color[idx1][1])*255;
	result.b = ((color[idx2][2] - color[idx1][2])*fractBetween + color[idx1][2])*255;
	return result;
}
