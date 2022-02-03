#pragma once

#include "Position.hpp"

class Snake;

enum class Object
{
    None,
    Snake,
    Food,
    Wall
};

class Map
{
private:
    Object **m_map;

public:
    Map();
    ~Map();
    const Position m_mapSize;

    Object get(const Position &p_position) const { return m_map[p_position.y][p_position.x]; }
    void set(const Position &p_position, Object p_object) { m_map[p_position.y][p_position.x] = p_object; }

    void AddFood();
    void AddWalls();
    void UpdateSnake(Snake *p_snake);
};
