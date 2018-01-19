#include "candle.hpp"
#include "materials.hpp"

using namespace units;

Candle::Candle()
{
	SetSize({21, 18}, 3);
}

void Candle::Reset()
{
	// Set origin to bottom center
	setOrigin(m_size.x * pixelsPerUnit / 2.0, m_size.y * pixelsPerUnit);

	m_points.clear();

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
	m_fire_level = 9;
	for(auto y = 0; y < m_fire_size.y; y++)
	{
		for(auto x = 0; x < m_fire_size.x; x++)
		{
			if(m_fire_image[y * m_fire_size.x + x])
			{
				const auto pos = sf::Vector2i((m_size.x / 2) - (m_fire_size.x / 2) + x, 4 + y);
				m_points.at(index(pos)).SetMaterial(Fire);
				m_points.at(index(pos)).temperature = 600.0;
			}
		}
	}

	// Paraffin
	for(auto y = m_fire_size.y + 4; y < m_size.y - m_iron_height; y++)
		for(auto x = 0; x < m_size.x; x++)
			m_points.at(y * m_size.x + x).SetMaterial(Paraffin);

	// Iron
	for(auto y = m_size.y - m_iron_height; y < m_size.y; y++)
		for(auto x = 0; x < m_size.x; x++)
			m_points.at(y * m_size.x + x).SetMaterial(Iron);

	m_backplate.setFillColor(getColorByMaterial(Paraffin) * sf::Color(215, 215, 215, 255));

	CalculateHeatmap();
}

double Candle::CalculateAverageDensity()
{
	double density_sum = 0;
	//TODO: Fix needed for omp
	//#pragma omp parallel for shared(density_sum)
	for(auto i = 0; i < m_size.x * m_size.y; ++i)
		density_sum += Config::Instance().Materials.at(m_points.at(i).GetMaterial()).density;

	return density_sum / static_cast<double>(m_size.x * m_size.y);
}

double Candle::CalculateTotalVolume()
{
	double volume = 0;
	//TODO: Fix needed for omp
	//#pragma omp parallel for shared(volume)
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

double Candle::CalculateWeight()
{
	double weight = 0;
	//TODO: Fix needed for omp
	//#pragma omp parallel for shared(weight)
	for(auto i = 0; i < m_size.x * m_size.y; ++i)
		weight += Config::Instance().Materials.at(m_points.at(i).GetMaterial()).density * pow(metersPerUnit, 3);
	
	return weight;
}

void Candle::MovePoint(sf::Vector2i old_pos, sf::Vector2i new_pos)
{
	m_points.at(index(new_pos)) = m_points.at(index(old_pos));
	m_points.at(index(new_pos)).setPosition({ static_cast<float>(new_pos.x) * pixelsPerUnit, static_cast<float>(new_pos.y) * pixelsPerUnit });
	m_points.at(index(old_pos)).setPosition({ static_cast<float>(old_pos.x) * pixelsPerUnit, static_cast<float>(old_pos.y) * pixelsPerUnit });

	m_points.at(index(old_pos)).SetMaterial(Air);
	m_points.at(index(old_pos)).temperature = 25.0;
}

void Candle::Update(sf::Time deltaTime)
{
	// Thermal conductivity
	#pragma omp parallel for
	for(auto i = 0; i < m_points.size(); i++)
	{
		// Fire temperature is constant. Don`t calculate.
		if(m_points.at(i).GetMaterial() == Fire || 
		   m_points.at(i).GetMaterial() == Iron)
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

			auto mat_vars = Config::Instance().Materials.at(mat);

			// Q = L * t * a^2 * tK
			const auto Q = mat_vars.thermalConductivity * deltaTime.asSeconds() * pow(a, 2) * (point.temperature - m_points.at(i).temperature);

			// delta temperature = Q / (c * m)
			const auto delta_t = Q / (mat_vars.heatCapacity * (mat_vars.density * pow(a, 3)));

			return m_points.at(i).temperature + delta_t;
		};

		// Calculate average temperature of parts and store it
		double temp_sum = 0;
		temp_sum += calculateNewTempWith(sf::Vector2i(0, -1));
		temp_sum += calculateNewTempWith(sf::Vector2i(0, 1));
		temp_sum += calculateNewTempWith(sf::Vector2i(1, 0));
		temp_sum += calculateNewTempWith(sf::Vector2i(-1, 0));
		m_points.at(i).temperature = temp_sum / 4.0;
	}

	// Paraffin aggregation state changing
	#pragma omp parallel for
	for(auto i = 0; i < m_points.size(); i++)
	{
		if(m_points.at(i).GetMaterial() == Paraffin && m_points.at(i).temperature > 65)
		{
			m_points.at(i).SetMaterial(ParaffinLiquid);
		}
		else if (m_points.at(i).GetMaterial() == ParaffinLiquid && m_points.at(i).temperature > 90)
		{
			m_points.at(i).SetMaterial(Air);
			m_points.at(i).temperature = 25;
		}
	}

	// TODO: Liquid paraffin trickling

	// Fire physics
	int fire_y;
	for (fire_y = 0; fire_y < m_size.y; ++fire_y)
	{
		auto point = m_points.at(index({ static_cast<int>(std::round(m_size.x / 2.0)), fire_y }));
		if(point.GetMaterial() == Paraffin ||
		   point.GetMaterial() == ParaffinLiquid)
			break;
	}
	
	if(fire_y != m_size.y)
	{
		// Move fire if levels are differents
		if(m_fire_level != fire_y)
		{
			const auto offset_x = (m_size.x / 2) - (m_fire_size.x / 2);

			for(auto y = m_fire_size.y - 1; y >= 0; --y)
			{
				for(auto x = 0; x < m_fire_size.x; ++x)
				{
					if(m_fire_image[y * m_fire_size.x + x])
					{
						MovePoint({ offset_x + x, m_fire_level - m_fire_size.y + y }, 
								  { offset_x + x, fire_y       - m_fire_size.y + y });
					}
				}
			}

			m_fire_level = fire_y;
		}
	}
	else
	{
		//Put out the fire
		for(auto y = m_fire_size.y - 1; y >= 0; --y)
			for(auto x = 0; x < m_fire_size.x; ++x)
				if(m_fire_image[y * m_fire_size.x + x])
					m_points.at(index({ (m_size.x / 2) - (m_fire_size.x / 2) + x, m_fire_level - m_fire_size.y + y })).SetMaterial(Air);
	}

	// Update backplate size
	int backplate_y_left;
	for(backplate_y_left = 0; backplate_y_left < m_size.y; ++backplate_y_left)
	{
		if(m_points.at(index(sf::Vector2i(0, backplate_y_left))).GetMaterial() == Paraffin)
			break;
	}

	int backplate_y_right;
	for (backplate_y_right = 0; backplate_y_right < m_size.y; ++backplate_y_right)
	{
		if(m_points.at(index(sf::Vector2i(m_size.x-1, backplate_y_right))).GetMaterial() == Paraffin)
			break;
	}

	m_backplate.setSize(sf::Vector2f(m_size.x * units::pixelsPerUnit, (m_size.y - std::min(backplate_y_right, backplate_y_left)) * units::pixelsPerUnit));
	m_backplate.setPosition(0, std::min(backplate_y_right, backplate_y_left) * units::pixelsPerUnit);
	
	if(Config::Instance().drawHeatmap)
		CalculateHeatmap();
}

size_t Candle::index(const sf::Vector2i pos) const
{
	return pos.y * m_size.x + pos.x;
}

void Candle::CalculateHeatmap()
{
	double t_min = m_points.at(0).temperature;
	double t_max = t_min;

	// Find max and min temperature
	for(auto& p : m_points)
	{
		if(p.temperature < t_min)
			t_min = p.temperature;
		if(p.temperature > t_max)
			t_max = p.temperature;
	}

	sf::Image heatmap_img;
	heatmap_img.create(m_size.x * units::pixelsPerUnit, m_size.y * units::pixelsPerUnit);

	// Create heatmap	
	#pragma omp parallel for shared(heatmap_img)
	for(auto i = 0; i < m_points.size(); ++i)
	{
		const auto pos = sf::Vector2f(i / m_size.x, i % m_size.x);

		const auto mix = (m_points.at(i).temperature - t_min) / (t_max - t_min);
		const auto pointColor = ColorMix(sf::Color::Blue, sf::Color::Red, mix) * sf::Color(255, 255, 255, 200);
		
		// Fill square
		for(auto y = 0; y < pixelsPerUnit; ++y)
			for(auto x = 0; x < pixelsPerUnit; ++x)
				heatmap_img.setPixel(pos.y * pixelsPerUnit + x, pos.x * pixelsPerUnit + y, pointColor);
	}

	// Set texture to rectangle
	static sf::Texture texture;
	texture.loadFromImage(heatmap_img);
	m_heatmap.setTexture(&texture);
}

void Candle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(m_backplate, states);
	for(const auto & point : m_points)
		target.draw(point, states);

	if(Config::Instance().drawHeatmap)
		target.draw(m_heatmap, states);
}
