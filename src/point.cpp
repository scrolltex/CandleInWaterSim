#include "point.hpp"

Point::Point() : Point(sf::Vector2i(0, 0), MaterialType::Air, AggregateState::Gas)
{

}

Point::Point(sf::Vector2i pos, MaterialType mat, AggregateState state)
{
    this->material = mat;
    this->aggregate = state;

    m_rectangle.setSize(sf::Vector2f(pixelsPerUnit, pixelsPerUnit));
    m_rectangle.setFillColor(sf::Color(255, 255, 255, 50));
    setPosition(pos);
}

Point::~Point()
{

}

void Point::setPosition(sf::Vector2i pos)
{
    m_position = pos;
    m_rectangle.setPosition(sf::Vector2f(pixelsPerUnit * m_position.x, pixelsPerUnit * m_position.y));
}

void Point::setMaterial(MaterialType mat)
{
    material = mat;
}

void Point::setAggregateState(AggregateState state)
{
    aggregate = state;
}

void Point::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_rectangle, states);
}
