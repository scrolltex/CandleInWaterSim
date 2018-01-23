#pragma once
#include <vector>
#include <cmath>
#include <functional>

#include <SFML/Graphics.hpp>

#include "point.hpp"
#include "units.hpp"
#include "config.hpp"

class Candle : public sf::Drawable, public sf::Transformable
{
public:
	/////////////////////////////////////////////////////////////
	// Singleton pattern ////////////////////////////////////////
	static Candle& Instance() 
	{
		static Candle instance;
		return instance;
	}

	Candle(Candle const&) = delete;             // Copy construct
	Candle(Candle&&) = delete;                  // Move construct
	Candle& operator=(Candle const&) = delete;  // Copy assign
	Candle& operator=(Candle &&) = delete;      // Move assign
	/////////////////////////////////////////////////////////////

	void Reset();

	sf::Vector2i GetSizeInPx() const
	{
		return (m_size - sf::Vector2i(0, m_fire_size.y + 4)) * units::pixelsPerUnit;
	}

	void SetSize(sf::Vector2i paraffin, int iron_height = 3)
	{
		m_size = paraffin;
		m_iron_height = iron_height;

		m_size.y += m_iron_height;
		m_size.y += m_fire_size.y;
		m_size.y += 4;

		m_heatmap.setSize(static_cast<sf::Vector2f>(m_size * units::pixelsPerUnit));

		Reset();
	}
	
	void SetWaterLevel(double level)
	{
		m_water_level = m_size.y - static_cast<int>(std::round(level / units::pixelsPerUnit));
	}

	double CalculateAverageDensity();

	double CalculateTotalVolume();

	double CalculateWeight();

	void MovePoint(sf::Vector2i old_pos, sf::Vector2i new_pos);
	
	void Update(sf::Time deltaTime);

	void CalculateFloating(sf::Vector2u wnd_size);

	Point GetPoint(sf::Vector2i pos) const
	{
		return m_points.at(index(pos));
	}

	std::function<void()> onCandleExtinguished;
	
private:
	Candle();
	~Candle() = default;

	inline size_t index(sf::Vector2i pos) const;

	void CalculateHeatmap();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::RectangleShape m_heatmap;
	std::vector<Point> m_points;

	sf::Vector2i m_size;
	int m_iron_height = 0;	

	sf::RectangleShape m_backplate;

	int m_water_level = 0;
	int m_fire_level = 0;

	const sf::Vector2i m_fire_size = { 5, 5 };
	const bool m_fire_image[25] = {
		// ReSharper disable CppCompileTimeConstantCanBeReplacedWithBooleanConstant
		0,0,1,0,0,
		0,1,1,1,0,
		1,1,1,1,1,
		1,1,1,1,1,
		0,1,1,1,0
		// ReSharper restore CppCompileTimeConstantCanBeReplacedWithBooleanConstant
	};
};
