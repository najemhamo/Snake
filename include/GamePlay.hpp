#pragma once

#include "State.hpp"
#include "Snake.hpp"

#include <curses.h>

class Map;
struct Context;

class GameMenu
{
public:
    GameMenu(SaveGameManager *saveGameManager);
    ~GameMenu();

    void SetMap(Map *p_map) { m_map = p_map; };
    void DrawBorder();
    void DrawElements();
    void Update();

    WINDOW *GetWindow();

private:
    WINDOW *m_border_win, *m_map_win;
    const Map *m_map;
    SaveGameManager* m_saveGameManager;

    void update_map();
};

class GamePlay : public State
{
public:
    GamePlay(Context *context);
    ~GamePlay();

    void Init() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    GameMenu m_menu;
    Map *m_map;
    Snake *m_snake;
};
