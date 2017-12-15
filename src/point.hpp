#pragma once
#include <SFML/Graphics.hpp>
#include "materials.hpp"

const int pixelsPerUnit = 8;

class Point : public sf::Drawable
{
public:
    Point();
    Point(sf::Vector2i pos, MaterialType mat);
    ~Point();

    void setPosition(sf::Vector2i pos);
    sf::Vector2i getPosition() const
    {
        return m_position;
    }

    void setMaterial(MaterialType mat);
    MaterialType getMaterial() const
    {
        return material;
    }

public:
    sf::Vector2i m_position;
    MaterialType material;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    sf::RectangleShape m_rectangle;
};
