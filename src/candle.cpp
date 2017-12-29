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
	setOrigin(m_size.x * pixelsPerUnit / 2.0, m_size.y * pixelsPerUnit);

	// Init points
	for(auto y = 0; y < m_size.y; y++)
	{
		for(auto x = 0; x < m_size.x; x++)
		{
			m_points.emplace_back(Air, 25.0);
			m_points.at(y * m_size.x + x).setPosition(x * pixelsPerUnit, y * pixelsPerUnit);
		}
	}
	
	// Fire
	const bool fire_image[] = {
		// ReSharper disable CppCompileTimeConstantCanBeReplacedWithBooleanConstant
		0,0,1,0,0,
		0,1,1,1,0,
		1,1,1,1,1,
		1,1,1,1,1,
		0,1,1,1,0
		// ReSharper restore CppCompileTimeConstantCanBeReplacedWithBooleanConstant
	};

	for(auto y = 0; y < fire_size.y; y++)
	{
		for(auto x = 0; x < fire_size.x; x++)
		{
			if(fire_image[y * fire_size.x + x])
			{
				const auto pos = sf::Vector2i((m_size.x/2) - (fire_size.x / 2) + x, 4 + y);
				m_points.at(index(pos)).SetMaterial(Fire);
				m_points.at(index(pos)).temperature = 600.0;
			}
		}
	}
	
	// Paraffin
	for(auto y = fire_size.y + 4; y < m_size.y - iron_height; y++)
		for(auto x = 0; x < m_size.x; x++)
			m_points.at(y * m_size.x + x).SetMaterial(Paraffin);

	// Iron
	for(auto y = m_size.y - iron_height; y < m_size.y; y++)
		for (auto x = 0; x < m_size.x; x++)
			m_points.at(y * m_size.x + x).SetMaterial(Iron);

	m_backplate.setFillColor(getColorByMaterial(Paraffin) * sf::Color(215, 215, 215, 255));
}

double Candle::CalculateAverageDensity()
{
	double density_sum = 0;
	for(auto i = 0; i < m_size.x * m_size.y; ++i)
		density_sum += getDensity(m_points.at(i).GetMaterial());

	return density_sum / static_cast<double>(m_size.x * m_size.y);
}

int Candle::CalculateTotalVolume()
{
	int volume = 0;
	for(auto i = 0; i < m_size.x * m_size.y; ++i)
	{
		const auto point = m_points.at(i);
		if(point.GetMaterial() == Paraffin || 
		   point.GetMaterial() == ParaffinLiquid || 
		   point.GetMaterial() == Iron)
		{
				volume++;
		}
	}

	return volume * metersPerUnit;
}

int Candle::CalculateWeight()
{
	double weight = 0;
	for(auto i = 0; i < m_size.x * m_size.y; ++i)
		weight += getDensity(m_points.at(i).GetMaterial()) * pow(metersPerUnit, 3);
	
	return weight;
}

void Candle::MovePoint(sf::Vector2i old_pos, sf::Vector2i new_pos)
{
	m_points.at(index(new_pos)) = m_points.at(index(old_pos));
	m_points.at(index(old_pos)).SetMaterial(Air);
	m_points.at(index(old_pos)).temperature = 25.0;
}

void Candle::Update(sf::Time deltaTime)
{
	// Thermal conductivity
	for(size_t i = 0; i < m_points.size() - m_size.x * 3; i++)
	{
		// Fire temperature is constant. Don`t calculate.
		if(m_points.at(i).GetMaterial() == Fire)
			continue;

		// New temp calculation of part
		auto calculateNewTempWith = [this, i, deltaTime](sf::Vector2i offset)
		{
			const auto pos = sf::Vector2i(i % m_size.x, i / m_size.x) + offset;
			const auto mat = m_points.at(i).GetMaterial();

			// Getting point. If point not in candle matrix, it is water or air
			Point point;
			if(pos.y < 0 || (pos.y < m_water_level && (pos.x < 0 || pos.x >= m_size.x)))
				point = Point(Air, 25.0);
			else if(pos.y >= m_size.y || (pos.y >= m_water_level && (pos.x < 0 || pos.x >= m_size.x)))
				point = Point(Water, 25.0);
			else
				point = m_points.at(index(pos));

			// Calculate new temperature of part
			const auto a = metersPerUnit / 4;

			// Q = L * t * a^2 * tK
			const auto Q = getThermalConductivity(mat) * deltaTime.asSeconds() * pow(a, 2) * (point.temperature - m_points.at(i).temperature);

			// delta temperature = Q / (c * m)
			const auto delta_t = Q / (getHeatCapacity(mat) * (getDensity(mat) * pow(a, 3)));

			return m_points.at(i).temperature + delta_t;
		};

		// Calculate average temperature of parts and store it
		double temp_sum = 0;
		temp_sum += calculateNewTempWith(sf::Vector2i(0, -1));
		temp_sum += calculateNewTempWith(sf::Vector2i(0, 1));
		temp_sum += calculateNewTempWith(sf::Vector2i(1, 0));
		temp_sum += calculateNewTempWith(sf::Vector2i(-1, 0));
		m_points.at(i).temperature = temp_sum / 4;
	}

	// Paraffin aggregation state changing
	for(auto & point : m_points)
	{
		if(point.GetMaterial() == Paraffin && point.temperature > 65)
		{
			point.SetMaterial(ParaffinLiquid);
		}
		else if (point.GetMaterial() == ParaffinLiquid && point.temperature > 90)
		{
			point.SetMaterial(Air);
			point.temperature = 25;
		}
	}

	// TODO: Liquid paraffin trickling

	// TODO: Fire physics

	// Update backplate size
	int backplate_y;
	for(backplate_y = 0; backplate_y < m_size.y; ++backplate_y)
	{
		if(m_points.at(index(sf::Vector2i(0, backplate_y))).GetMaterial() == Paraffin)
			break;
	}

	m_backplate.setSize(sf::Vector2f(m_size.x * units::pixelsPerUnit, (m_size.y - backplate_y) * units::pixelsPerUnit));
	m_backplate.setPosition(0, backplate_y * units::pixelsPerUnit);
}

size_t Candle::index(const sf::Vector2i pos) const
{
	return pos.y * m_size.x + pos.x;
}

void Candle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_backplate, states);
	for(const auto & point : m_points)
		target.draw(point, states);
}
