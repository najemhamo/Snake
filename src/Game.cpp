#include "Game.hpp"
#include "Settings.hpp"
#include "State.hpp"
#include "StateManager.hpp"
#include "SaveGameManager.hpp"
#include "Context.hpp"
#include "MainMenu.hpp"

#include <curses.h>

Position Settings::map_size = {25, 50};
bool Settings::enable_walls = true;

Game::Game()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    keypad(stdscr, true);
}

Game::~Game()
{
    endwin();
}

void Game::Run()
{
    StateManager stateManager;
    SaveGameManager saveGameManager;

    Context context = {&stateManager, &saveGameManager};
    stateManager.Add(std::make_unique<MainMenu>(&context));

    stateManager.ProcessStateChange();
    stateManager.GetCurrentState()->Init();

    while (!stateManager.IsEmpty())
    {
        if (!stateManager.GetCurrentState()->IsPaused())
        {
            stateManager.GetCurrentState()->Update();
        }
        stateManager.GetCurrentState()->Draw();
        stateManager.GetCurrentState()->ProcessInputs();
        stateManager.ProcessStateChange();
    }
}