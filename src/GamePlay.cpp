#include "GamePlay.hpp"

#include "MainMenu.hpp"
#include "StateManager.hpp"
#include "Context.hpp"
#include "Map.hpp"
#include "Settings.hpp"
#include "SaveGameManager.hpp"

#include <curses.h>
#include <chrono>
#include <thread>

namespace MenuUtils
{
    int dialogbox(std::string p_text, std::vector<std::string> p_clicks)
    {
        // if COLS / 4 < min_width(the width so that all elements would fit) -> width = COLS - 4, else width = COLS / 4
        int width = COLS / 4 < [&p_text, &p_clicks]() -> int
        {
            int min_width = 0;
            for (std::string click : p_clicks)
            {
                min_width += click.length() + 2;
            }
            min_width = min_width > (int)p_text.length() ? min_width : p_text.length();
            return min_width + 10;
        }()
                                                             ? COLS - 10
                                                             : COLS / 4;

        WINDOW *win = newwin(7, width, (LINES - 7) / 2, (COLS - (width)) / 2);
        keypad(win, true);

        box(win, 0, 0);
        mvwprintw(win, 2, (win->_maxx - p_text.length()) / 2, p_text.c_str());
        wrefresh(win);

        int selected_option = 0;
        while (true)
        {
            for (std::size_t i = 0; i < p_clicks.size(); ++i)
            {
                // x = (total width of the window / (amount of clicks + 1)) * (current click + 1) - (length of the text of the click / 2)
                mvwprintw(win,
                          5,
                          (win->_maxx / (p_clicks.size() + 1)) * (i + 1) - (p_clicks[i].length() / 2),
                          p_clicks[i].c_str());
            }

            mvwchgat(win, 5, (win->_maxx / (p_clicks.size() + 1)) * (selected_option + 1) - (p_clicks[selected_option].length() / 2), p_clicks[selected_option].length(), A_STANDOUT, 0, NULL);

            switch (wgetch(win))
            {
            case KEY_LEFT:
                selected_option = selected_option != 0 ? selected_option - 1 : p_clicks.size() - 1;
                break;
            case KEY_RIGHT:
                selected_option = selected_option != (int)p_clicks.size() - 1 ? selected_option + 1 : 0;
                break;
            // Enter
            case '\n':
                werase(win);
                wrefresh(win);
                delwin(win);
                return selected_option;
            }
        }
    }
};

GamePlay::GamePlay(Context *context)
    : State(context), m_menu(context->saveGameManager)
{
    m_map = new Map();
    m_menu.SetMap(m_map);
    m_snake = new Snake(context->saveGameManager);
    m_context->saveGameManager->ResetScore();
}

GamePlay::~GamePlay()
{
    delete m_map;
    delete m_snake;
}

void GamePlay::Init()
{
    if (Settings::enable_walls)
    {
        m_map->AddWalls();
    }
    m_map->AddFood();
}

void GamePlay::Update()
{
    const Position &snake_head = m_snake->get();
    switch (m_map->get(snake_head))
    {
    case Object::Food:
    {
        m_map->set(snake_head, Object::Snake);
        m_map->AddFood();
        m_snake->lengthen();
        break;
    }
    case Object::Wall:
    case Object::Snake:
    {
        MenuUtils::dialogbox(std::string("You died"), std::vector<std::string>{std::string("OK")});
        m_context->saveGameManager->Save();
        m_context->stateManager->Add(std::make_unique<MainMenu>(m_context));
        break;
    }
    case Object::None:
        break;
    }

    m_map->UpdateSnake(m_snake);
    m_snake->move(m_map->m_mapSize);
    m_menu.Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(145));
}

void GamePlay::Draw()
{
    m_menu.DrawBorder();
    m_menu.DrawElements();
}

void GamePlay::ProcessInputs()
{
    Meeting meeting = Meeting::null;
    switch (wgetch(m_menu.GetWindow()))
    {
    case KEY_UP:
        meeting = Meeting::up;
        break;
    case KEY_RIGHT:
        meeting = Meeting::right;
        break;
    case KEY_DOWN:
        meeting = Meeting::down;
        break;
    case KEY_LEFT:
        meeting = Meeting::left;
        break;
    case 'q':

        if (MenuUtils::dialogbox(std::string("Quit?"), std::vector<std::string>{std::string("No"), std::string("Yes")}) == 1)
        {
            m_context->saveGameManager->Save();
            m_context->stateManager->Add(std::make_unique<MainMenu>(m_context));
        }
        break;
    }
    m_snake->set_meeting(meeting);
}

GameMenu::GameMenu(SaveGameManager* saveGameManager) : m_saveGameManager(saveGameManager)
{
    m_border_win = newwin(Settings::map_size.y + 3, Settings::map_size.x + 2, (LINES - Settings::map_size.y) / 2 - 2, (COLS - Settings::map_size.x) / 2 - 1);
    m_map_win = newwin(Settings::map_size.y, Settings::map_size.x, (LINES - Settings::map_size.y) / 2, (COLS - Settings::map_size.x) / 2);
    nodelay(m_map_win, true);
    keypad(m_map_win, true);
}

GameMenu::~GameMenu()
{
    delwin(m_map_win);
    delwin(m_border_win);
}

void GameMenu::DrawBorder()
{
    box(m_border_win, 0, 0);
    wrefresh(m_border_win);
}

void GameMenu::DrawElements()
{
    for (int row = 0; row < m_map->m_mapSize.y; ++row)
    {
        for (int col = 0; col < m_map->m_mapSize.x; ++col)
        {
            if (m_map->get({row, col}) == Object::Wall)
            {
                mvwaddch(m_map_win, row, col, '#');
            }
        }
    }

    wrefresh(m_map_win);
}

void GameMenu::Update()
{
    mvwprintw(m_border_win, 0, 2, "Score: %d", m_saveGameManager->GetScore());
    mvwprintw(m_border_win, 1, 2, "Highscore: %d", m_saveGameManager->GetHighScore());
    wrefresh(m_border_win);
    update_map();
    wrefresh(m_map_win);
    m_saveGameManager->Update();
}

void GameMenu::update_map()
{
    for (int row = 0; row < m_map->m_mapSize.y; ++row)
    {
        for (int col = 0; col < m_map->m_mapSize.x; ++col)
        {
            switch (m_map->get({row, col}))
            {
            case Object::None:
                mvwaddch(m_map_win, row, col, ' ');
                break;
            case Object::Snake:
                mvwaddch(m_map_win, row, col, '*');
                break;
            case Object::Food:
                mvwaddch(m_map_win, row, col, '$');
                break;
            default:
                break;
            }
        }
    }
}

WINDOW *GameMenu::GetWindow()
{
    return m_map_win;
}
