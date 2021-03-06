#include "point.hpp"
#include "units.hpp"

using namespace units;

Point::Point() : Point(Air, 25.0)
{
	
}

Point::Point(MaterialType mat, double temperature)
{
    m_rectangle.setSize(sf::Vector2f(pixelsPerUnit, pixelsPerUnit));
    SetMaterial(mat);
	this->temperature = temperature;
}

void Point::SetMaterial(MaterialType mat)
{
    m_material = mat;
    m_rectangle.setFillColor(getColorByMaterial(m_material));
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
    target.draw(m_rectangle, states);
}
