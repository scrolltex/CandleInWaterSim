#include "candle.hpp"
#include "materials.hpp"

Candle::Candle()
{
	// Setup size
	m_size = sf::Vector2i(21, 18);

	const int iron_height = 3;
	const sf::Vector2i fire_size(5, 5);

	m_size.y += iron_height;
	m_size.y += fire_size.y + 4;
	
	m_water_level = 0;

	// Set origin to bottom center
	setOrigin(m_size.x * pixelsPerUnit / 2, m_size.y * pixelsPerUnit);

	// Init points
	for(auto y = 0; y < m_size.y; y++)
	{
		for(auto x = 0; x < m_size.x; x++)
		{
			m_points.push_back(Point(Air));
			m_points.at(y * m_size.x + x).setPosition(x * pixelsPerUnit, y * pixelsPerUnit);
		}
	}
	
	// Fire
	const bool fire_image[] = {
		0,0,1,0,0,
		0,1,1,1,0,
		1,1,1,1,1,
		1,1,1,1,1,
		0,1,1,1,0
	};

	for(auto y = 0; y < fire_size.y; y++)
	{
		for(auto x = 0; x < fire_size.x; x++)
		{
			if(fire_image[y * fire_size.x + x])
			{
				const auto pos = sf::Vector2i((m_size.x/2) - (fire_size.x / 2) + x, 4 + y);
				m_points.at(pos.y * m_size.x + pos.x).setMaterial(Fire);
			}
		}
	}

	// Paraffin
	for(auto y = fire_size.y + 4; y < m_size.y - iron_height; y++)
		for(auto x = 0; x < m_size.x; x++)
			m_points.at(y * m_size.x + x).setMaterial(Paraffin);

	// Iron
	for(auto y = m_size.y - iron_height; y < m_size.y; y++)
		for (auto x = 0; x < m_size.x; x++)
			m_points.at(y * m_size.x + x).setMaterial(Iron);
}

Candle::~Candle()
{

}

double Candle::CalculateAverageDensity()
{
	double density_sum = 0;
	for (auto y = 0; y < m_size.y; y++)
		for (auto x = 0; x < m_size.x; x++)
		density_sum += getDensityByMaterial(m_points.at(y * m_size.x + x).getMaterial());

	return density_sum / static_cast<double>(m_size.x + m_size.y);
}

int Candle::CalculateTotalVolume()
{
	int volume = 0;
	for(auto y = 0; y < m_size.y; y++)
	{
		for(auto x = 0; x < m_size.x; x++)
		{
			const auto point = m_points.at(y * m_size.x + x);
			if(point.getMaterial() == Paraffin || 
			   point.getMaterial() == ParaffinLiquid || 
			   point.getMaterial() == Iron)
			{
				volume++;
			}
		}
	}

	return volume * cmPerUnit;
}

int Candle::CalculateWeight()
{
	double weight = 0;
	for(auto y = 0; y < m_size.y; y++)
		for(auto x = 0; x < m_size.x; x++)
			weight += getDensityByMaterial(m_points.at(y * m_size.x + x).getMaterial()) * cmPerUnit;

	return weight;
}

void Candle::MovePoint(sf::Vector2i old_pos, sf::Vector2i new_pos)
{
	m_points.at(new_pos.y * m_size.x + new_pos.x) = m_points.at(old_pos.y * m_size.x + old_pos.x);
	m_points.at(old_pos.y * m_size.x + old_pos.x) = Point(Air);
}

void Candle::update(sf::Time deltaTime)
{
	//TODO: Physic calculations
}

void Candle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for(auto y = 0; y < m_size.y; y++)
		for(auto x = 0; x < m_size.x; x++)
			target.draw(m_points.at(y * m_size.x + x), states);
}
