#pragma once

#include <vector>

#include "Position.hpp"

class SaveGameManager;

// Snake Directions
enum class Meeting
{
    right,
    down,
    left,
    up,
    null
};

class Snake
{
public:
    Snake(SaveGameManager* saveGameManager);
    ~Snake() = default;
    void move(Position p_map_size);
    void lengthen();
    Position get(unsigned int p_at = 0) { return m_position.at(p_at); }

    Meeting get_meeting() { return m_meeting; }
    void set_meeting(Meeting p_meeting);

    // returns the amount of the snake size
    int size() { return m_position.size(); }

    // returns the snake's length. size() may have not been updated to it yet
    unsigned int length() { return m_length; }

private:
    std::vector<Position> m_position{{5, 5}};
    unsigned int m_length = 1;
    Meeting m_meeting = Meeting::right;
    SaveGameManager* m_saveGameManager;
};
