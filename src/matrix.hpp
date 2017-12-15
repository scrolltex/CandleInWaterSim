#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "point.hpp"

class Matrix : public sf::Drawable
{
public:
    Matrix(int width, int height);
    ~Matrix();

    void update(sf::Time deltaTime);

    Point* getPoint(int x, int y) const;

private:
    void SetupGrid();
    void SetupMatrix();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Vector2i m_size;
    Point *m_points;
    std::vector<sf::Vertex> m_grid;
};
