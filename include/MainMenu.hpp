#pragma once

#include "State.hpp"
#include "MenuOption.hpp"

#include <array>

struct Context;
class MainMenu : public State
{
public:
    MainMenu(Context* context);
    ~MainMenu() = default;

    void Init() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    void Navigate();

private:
    std::array<MenuOption, 3> m_menuOptions;
    std::string m_title = std::string("2D Snake Game || By Najem Hamo");
    std::size_t m_selectedIndex = 0;
};
