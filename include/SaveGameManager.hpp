#pragma once

#include <fstream>

class SaveGameManager
{
public:
    SaveGameManager();
    ~SaveGameManager();

    void AddToScore(int points);
    void Save();
    int GetScore() const;
    int GetHighScore() const;
    void ResetScore();
    void Update();

private:
    std::fstream m_file;
    int m_score = 0;
    int m_highScore = 0;
};
