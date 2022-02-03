#include "MainMenu.hpp"
#include "StateManager.hpp"
#include "Context.hpp"
#include "GamePlay.hpp"
#include "SettingsMenu.hpp"

#include <curses.h>

MainMenu::MainMenu(Context *context)
    : State(context)
{
}

void MainMenu::Init()
{
    m_menuOptions[0] = {"New Game", {}};
    m_menuOptions[1] = {"Settings", {}};
    m_menuOptions[2] = {"Exit", {}};
}

void MainMenu::Update()
{
    for (std::size_t i = 0; i < m_menuOptions.size(); ++i)
    {
        m_menuOptions[i].position = {LINES / 2 + (int)i,
                                     (COLS - (int)m_menuOptions[i].label.length()) / 2};
    }
}

void MainMenu::Draw()
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

void MainMenu::Navigate()
{
    switch (m_selectedIndex)
    {
    case 0:
        m_context->stateManager->Add(std::make_unique<GamePlay>(m_context));
        break;
    case 1:
        m_context->stateManager->Add(std::make_unique<SettingsMenu>(m_context));
        break;
    case 2:
        m_context->stateManager->PopCurrent();
        break;
    }
}

void MainMenu::ProcessInputs()
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
        this->Navigate();
        break;
    case 'q':
        m_context->stateManager->PopCurrent();
        break;
    }
}
