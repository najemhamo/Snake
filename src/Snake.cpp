#include "Snake.hpp"
#include "SaveGameManager.hpp"

#include <stdexcept>

Snake::Snake(SaveGameManager *saveGameManager) : m_saveGameManager(saveGameManager)
{
}

void Snake::move(Position p_map_size)
{
    switch (m_meeting)
    {
    case Meeting::right:
    {
        if (m_position[0].x + 1 == p_map_size.x)
            m_position.insert(m_position.begin(), {m_position.front().y, 0});
        else
            m_position.insert(m_position.begin(), {m_position.front().y, m_position.front().x + 1});
        break;
    }
    case Meeting::down:
    {
        if (m_position[0].y + 1 == p_map_size.y)
            m_position.insert(m_position.begin(), {0, m_position.front().x});
        else
            m_position.insert(m_position.begin(), {m_position.front().y + 1, m_position.front().x});
        break;
    }
    case Meeting::left:
    {
        if (m_position[0].x - 1 == -1)
            m_position.insert(m_position.begin(), {m_position.front().y, p_map_size.x - 1});
        else
            m_position.insert(m_position.begin(), {m_position.front().y, m_position.front().x - 1});
        break;
    }
    case Meeting::up:
    {
        if (m_position[0].y - 1 == -1)
            m_position.insert(m_position.begin(), {p_map_size.y - 1, m_position.front().x});
        else
            m_position.insert(m_position.begin(), {m_position.front().y - 1, m_position.front().x});
        break;
    }
    default:
    {
        throw std::invalid_argument("Snake has wrong meeting");
    }
    }

    if (m_position.size() > m_length)
        m_position.pop_back();
}

void Snake::set_meeting(Meeting p_meeting)
{
    switch (p_meeting)
    {
    case Meeting::right:
        if (m_meeting != Meeting::left)
            m_meeting = p_meeting;
        break;
    case Meeting::left:
        if (m_meeting != Meeting::right)
            m_meeting = p_meeting;
        break;
    case Meeting::down:
        if (m_meeting != Meeting::up)
            m_meeting = p_meeting;
        break;
    case Meeting::up:
        if (m_meeting != Meeting::down)
            m_meeting = p_meeting;
        break;
    default:
        break;
    }
}

void Snake::lengthen()
{
    m_length++;
    m_saveGameManager->AddToScore(1);
}
