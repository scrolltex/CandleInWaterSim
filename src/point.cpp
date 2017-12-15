#include "point.hpp"

Point::Point() : Point(sf::Vector2i(0, 0), MaterialType::Air)
{

}

Point::Point(sf::Vector2i pos, MaterialType mat)
{
    m_rectangle.setSize(sf::Vector2f(pixelsPerUnit - 2, pixelsPerUnit - 2));
    setMaterial(mat);
    setPosition(pos);
}

Point::~Point()
{

}

void Point::setPosition(sf::Vector2i pos)
{
    this->m_position = pos;
    m_rectangle.setPosition(sf::Vector2f(pixelsPerUnit * m_position.x + 1, pixelsPerUnit * m_position.y + 1));
}

void Point::setMaterial(MaterialType mat)
{
    this->material = mat;
    m_rectangle.setFillColor(getColorByMaterial(this->material));
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_rectangle, states);
}
