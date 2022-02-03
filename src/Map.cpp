#include <random>

#include "Map.hpp"
#include "Snake.hpp"
#include "Settings.hpp"

Map::Map() : m_mapSize(Settings::map_size)
{
    m_map = new Object *[m_mapSize.y];
    for (int row = 0; row < m_mapSize.y; row++)
    {
        m_map[row] = new Object[m_mapSize.x];
    }

    for (int y = 0; y < m_mapSize.y; ++y)
    {
        for (int x = 0; x < m_mapSize.x; ++x)
        {
            m_map[y][x] = Object::None;
        }
    }
}

Map::~Map()
{
    for (int row = 0; row < m_mapSize.y; row++)
        delete[] m_map[row];
    delete[] m_map;
}

void Map::AddFood()
{
    //Spawn the food randomly
    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> disty(0, m_mapSize.y - 1);
    std::uniform_int_distribution<int> distx(0, m_mapSize.x - 1);
    Position location = {disty(rng), distx(rng)};
    while (get(location) != Object::None)
    {
        location = {disty(rng), distx(rng)};
    }
    set(location, Object::Food);
}

//Draw the walls in Y and X
void Map::AddWalls()
{

    for (int y = 0; y < m_mapSize.y; y++)
    {
        m_map[y][0] = Object::Wall;
        m_map[y][m_mapSize.x - 1] = Object::Wall;
    }

    for (int x = 0; x < m_mapSize.x; x++)
    {
        m_map[0][x] = Object::Wall;
        m_map[m_mapSize.y - 1][x] = Object::Wall;
    }
}

void Map::UpdateSnake(Snake *p_snake)
{
    for (int row = 0; row < m_mapSize.y; ++row)
    {
        for (int col = 0; col < m_mapSize.x; ++col)
        {
            if (m_map[row][col] == Object::Snake)
            {
                m_map[row][col] = Object::None;
            }
        }
    }

    for (int i = 0; i < p_snake->size(); ++i)
    {
        Position snake_position = p_snake->get(i);
        m_map[snake_position.y][snake_position.x] = Object::Snake;
    }
}