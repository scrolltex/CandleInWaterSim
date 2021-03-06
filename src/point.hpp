#pragma once
#include <SFML/Graphics.hpp>
#include "materials.hpp"

class Point : public sf::Drawable, public sf::Transformable
{
public:
	Point();
	Point(MaterialType mat, double temperature);
    ~Point() = default;
	
    void SetMaterial(MaterialType mat);
    MaterialType GetMaterial() const
    {
        return m_material;
    }

	double temperature;
	
private:
    MaterialType m_material;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    sf::RectangleShape m_rectangle;
};
