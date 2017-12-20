#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "materials.hpp"
#include "point.hpp"

class Entity
{
public:
	Entity() { }
	~Entity() { }

	double CalculateAverageDensity()
	{
		double density_sum = 0;
		for(auto i = 0; i < points.size(); i++)
			density_sum += getDensityByMaterial(points.at(i)->getMaterial());

		return density_sum / static_cast<double>(points.size());
	}

	int CalculateTotalV()
	{
		return points.size() * cmPerUnit;
	}

	int CalculateWeight()
	{
		double weight = 0;
		for(auto i = 0; i < points.size(); i++)
			weight += getDensityByMaterial(points.at(i)->getMaterial()) * cmPerUnit;

		return weight;
	}

	std::vector<std::shared_ptr<Point>> points;

	sf::Vector2i size;
};
