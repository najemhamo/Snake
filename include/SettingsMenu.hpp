#pragma once

#include "State.hpp"
#include "MenuOption.hpp"

#include <array>

struct Context;

class SettingsMenu : public State
{
public:
    SettingsMenu(Context* context);
    ~SettingsMenu() = default;

    void Init() override;
    void Update() override;
    void Draw() override;
    void ProcessInputs() override;

private:
    void HandleInput();

private:
    std::array<MenuOption, 3> m_menuOptions;
    std::string m_title = std::string("Settings");
    std::size_t m_selectedIndex = 0;
};
