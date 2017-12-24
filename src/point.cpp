#include "point.hpp"
#include "units.hpp"

Point::Point() : Point(Air)
{
	
}

Point::Point(MaterialType mat)
{
    m_rectangle.setSize(sf::Vector2f(pixelsPerUnit, pixelsPerUnit));
    setMaterial(mat);
}

Point::~Point()
{

}

void Point::setMaterial(MaterialType mat)
{
    m_material = mat;
    m_rectangle.setFillColor(getColorByMaterial(m_material));
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
    target.draw(m_rectangle, states);
}
