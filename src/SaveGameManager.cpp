#include "SaveGameManager.hpp"
#include <cassert>

SaveGameManager::SaveGameManager()
{
    m_file.open("save.snake", std::ios::in | std::ios::binary);
    if(m_file)
    {
        m_file.read((char*)&m_highScore, sizeof(int));
    }
    else
    {
        m_score = 0;
        m_highScore = 0;
    }
    m_file.close();
}

SaveGameManager::~SaveGameManager()
{
    m_file.close();
}

void SaveGameManager::AddToScore(int points)
{
    // Are you sure you want to add (-)ve points.
    assert(points >= 0);
    m_score += points;
}

void SaveGameManager::Save()
{
    if(m_score >= m_highScore)
    {
        m_file.open("save.snake", std::ios::out | std::ios::binary);
        if(m_file)
        {
            m_file.write((char*)&m_score, sizeof(int));
            m_file.close();
        }
        m_highScore = m_score;
    }
}

int SaveGameManager::GetScore() const
{
    return m_score;
}

int SaveGameManager::GetHighScore() const
{
    return m_highScore;
}

void SaveGameManager::ResetScore()
{
    m_score = 0;
}

void SaveGameManager::Update()
{
    if(m_score > m_highScore)
    {
        m_highScore = m_score;
    }
}