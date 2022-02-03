#pragma once

class StateManager;
class SaveGameManager;

struct Context
{
    StateManager *stateManager;
    SaveGameManager *saveGameManager;
};
