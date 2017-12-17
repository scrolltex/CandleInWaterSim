#include "matrix.hpp"

Matrix::Matrix(int width, int height)
{
    m_size.x = width;
    m_size.y = height;

    // Allocate array of points
    m_points = new Point[m_size.x * m_size.y];

    // Setup points positions
    for(auto y = 0; y < m_size.y; y++)
        for(auto x = 0; x < m_size.x; x++)
            getPoint(x, y)->setPosition(sf::Vector2i(x, y));

    SetupGrid();
    SetupMatrix();
}

Matrix::~Matrix()
{
    delete [] m_points;
}

void Matrix::SetupGrid()
{
    const sf::Color lineColor(220, 220, 220);
    
    // Vertical
    for(auto i = 0; i < m_size.x - 1; i++)
    {
        m_grid.push_back(sf::Vertex(sf::Vector2f(pixelsPerUnit + pixelsPerUnit * i, 0), lineColor));    
        m_grid.push_back(sf::Vertex(sf::Vector2f(pixelsPerUnit + pixelsPerUnit * i, m_size.y * pixelsPerUnit), lineColor));
    }

    // Horizontal
    for(auto i = 0; i < m_size.y - 1; i++)
    {
        m_grid.push_back(sf::Vertex(sf::Vector2f(0, pixelsPerUnit + pixelsPerUnit * i), lineColor));    
        m_grid.push_back(sf::Vertex(sf::Vector2f(m_size.x * pixelsPerUnit, pixelsPerUnit + pixelsPerUnit * i), lineColor));
    }

    // Top
    m_grid.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black));
    m_grid.push_back(sf::Vertex(sf::Vector2f(m_size.x * pixelsPerUnit, 0), sf::Color::Black));

    // Bottom
    m_grid.push_back(sf::Vertex(sf::Vector2f(0, m_size.y * pixelsPerUnit), sf::Color::Black));
    m_grid.push_back(sf::Vertex(sf::Vector2f(m_size.x * pixelsPerUnit, m_size.y * pixelsPerUnit), sf::Color::Black));
    
    // Left
    m_grid.push_back(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Black));
    m_grid.push_back(sf::Vertex(sf::Vector2f(0, m_size.y * pixelsPerUnit), sf::Color::Black));
   
    // Right
    m_grid.push_back(sf::Vertex(sf::Vector2f(m_size.x * pixelsPerUnit, 0), sf::Color::Black));
    m_grid.push_back(sf::Vertex(sf::Vector2f(m_size.x * pixelsPerUnit, m_size.y * pixelsPerUnit), sf::Color::Black));
}

void Matrix::SetupMatrix()
{
    // Fill water
    for(auto y = m_size.y/2; y < m_size.y; y++)
        for(auto x = 0; x < m_size.x; x++)
            getPoint(x, y)->setMaterial(MaterialType::Water);

    // TODO: Setup candle
	sf::Vector2i candle_size(16, 16);
	sf::Vector2i iron_size(candle_size.x, 3);
	
	const sf::Vector2i center = m_size / 2;

	// Paraffin
	for(auto y = 0; y < candle_size.y; y++)
		for(auto x = 0; x < candle_size.x; x++)
			getPoint(center.x - (candle_size.x / 2) + x, center.y + y)->setMaterial(MaterialType::Paraffin);

	// Iron
	for(auto y = 0; y < iron_size.y; y++)
		for(auto x = 0; x < iron_size.x; x++)
			getPoint(center.x - (iron_size.x / 2) + x, center.y + candle_size.y + y)->setMaterial(MaterialType::Iron);

	// Fire
	sf::Vector2i fire_size(4, 5);
	bool fire[] = {
		0,1,1,0,
		0,1,1,0,
		1,1,1,1,
		1,1,1,1,
		0,1,1,0
	};
	for(auto y = 0; y < fire_size.y; y++)
	{
		for(auto x = 0; x < fire_size.x; x++)
		{
			if(fire[y * fire_size.x + x])
				getPoint(center.x - (fire_size.x / 2) + x, center.y - fire_size.y + y)->setMaterial(MaterialType::Fire);
		}
	}
}

void Matrix::update(sf::Time deltaTime)
{

}

Point* Matrix::getPoint(int x, int y) const
{
    return &m_points[x * m_size.y + y];
}

void Matrix::draw(sf::RenderTarget& target, sf::RenderStates states) const
{    
    target.draw(&m_grid[0], m_grid.size(), sf::Lines, states);
    for(auto i = 0; i < m_size.x * m_size.y; i++)
        target.draw(m_points[i], states);
}
