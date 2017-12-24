#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "point.hpp"
#include "units.hpp"

class Candle : public sf::Drawable, public sf::Transformable
{
public:
	Candle();
	~Candle();

	sf::Vector2i getSizeInPx() const
	{
		return (m_size - sf::Vector2i(0, 9)) * pixelsPerUnit;
	}
	
	void SetWaterLevel(double level)
	{
		m_water_level = level / pixelsPerUnit;
	}

	double CalculateAverageDensity();

	int CalculateTotalVolume();

	int CalculateWeight();

	void MovePoint(sf::Vector2i old_pos, sf::Vector2i new_pos);
	
	void update(sf::Time deltaTime);
	
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	std::vector<Point> m_points;

	sf::Vector2i m_size;

	int m_water_level;
};
