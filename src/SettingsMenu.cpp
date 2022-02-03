#include "SettingsMenu.hpp"
#include "StateManager.hpp"
#include "Context.hpp"
#include "GamePlay.hpp"
#include "Settings.hpp"
#include "MainMenu.hpp"

#include <curses.h>

static std::string boolToString(bool flag)
{
    return flag ? "Enabled" : "Disabled";
}

SettingsMenu::SettingsMenu(Context *context)
    : State(context)
{
}

void SettingsMenu::Init()
{
    m_menuOptions[0] = {"Map size: " +
                            std::to_string(Settings::map_size.y) +
                            " rows, " +
                            std::to_string(Settings::map_size.x) +
                            " cols",
                        {}};
    m_menuOptions[1] = {"Walls: " + boolToString(Settings::enable_walls), {}};
    m_menuOptions[2] = {"Back", {}};
}

void SettingsMenu::Update()
{
    for (std::size_t i = 0; i < m_menuOptions.size(); ++i)
    {
        m_menuOptions[i].position = {LINES / 2 + (int)i,
                                     (COLS - (int)m_menuOptions[i].label.length()) / 2};
    }
}

void SettingsMenu::Draw()
{
    erase();

    mvprintw(LINES / 4, (COLS - m_title.length()) / 2, m_title.c_str());

    for (const auto &option : m_menuOptions)
    {
        mvprintw(option.position.y, option.position.x, option.label.c_str());
    }

    mvchgat(m_menuOptions[m_selectedIndex].position.y,
            m_menuOptions[m_selectedIndex].position.x,
            m_menuOptions[m_selectedIndex].label.length(),
            A_STANDOUT,
            0,
            NULL);

    refresh();
}

void SettingsMenu::HandleInput()
{
    switch (m_selectedIndex)
    {
    case 0:
        switch (Settings::map_size.y)
        {
        case 18:
            Settings::map_size = {25, 50};
            break;
        case 25:
            Settings::map_size = {10, 20};
            break;
        default:
            Settings::map_size = {18, 35};
            break;
        }
        m_menuOptions[0].label = std::string("Map size: ") + std::to_string(Settings::map_size.y) + " rows, " + std::to_string(Settings::map_size.x) + " cols";
        break;
    case 1:
        Settings::enable_walls = !Settings::enable_walls;
        m_menuOptions[1].label = "Walls: " + boolToString(Settings::enable_walls);
        break;
    case 2:
        m_context->stateManager->Add(std::make_unique<MainMenu>(m_context));
        break;
    }
}

void SettingsMenu::ProcessInputs()
{
    switch (getch())
    {
    case KEY_UP:
        m_selectedIndex = m_selectedIndex != 0 ? m_selectedIndex - 1 : m_menuOptions.size() - 1;
        break;
    case KEY_DOWN:
        m_selectedIndex = m_selectedIndex != m_menuOptions.size() - 1 ? m_selectedIndex + 1 : 0;
        break;
    case '\n':
        this->HandleInput();
        break;
    case 'q':
        m_context->stateManager->Add(std::make_unique<MainMenu>(m_context));
        break;
    }
}
