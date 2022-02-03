#pragma once

struct Position
{
    int y;
    int x;

    bool operator==(const Position &other)
    {
        return this->y == other.y &&
               this->x == other.x;
    }
};
