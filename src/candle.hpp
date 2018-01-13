#pragma once
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "point.hpp"
#include "units.hpp"

using namespace units;

const static bool fire_image[25] = {
	0,0,1,0,0,
	0,1,1,1,0,
	1,1,1,1,1,
	1,1,1,1,1,
	0,1,1,1,0
};

class Candle : public sf::Drawable, public sf::Transformable
{
public:
	Candle();
	~Candle() = default;

	void Reset();

	sf::Vector2i GetSizeInPx() const
	{
		return (m_size - sf::Vector2i(0, 9)) * pixelsPerUnit;
	}
	
	void SetWaterLevel(double level)
	{
		m_water_level = m_size.y - static_cast<int>(std::round(level / pixelsPerUnit));
	}

	double CalculateAverageDensity();

	double CalculateTotalVolume();

	double CalculateWeight();

	void MovePoint(sf::Vector2i old_pos, sf::Vector2i new_pos);
	
	void Update(sf::Time deltaTime);
	
private:
	inline size_t index(sf::Vector2i pos) const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	std::vector<Point> m_points;
	sf::Vector2i m_size;

	sf::RectangleShape m_backplate;

	int m_water_level;

	sf::Vector2i m_fire_size = { 5, 5 };
	int m_fire_level = 0;
	const int iron_height = 3;
};
