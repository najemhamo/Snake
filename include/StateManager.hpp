#pragma once

#include <stack>
#include <memory>

class State;

class StateManager
{
public:
    StateManager() = default;
    ~StateManager() = default;

    void Add(std::unique_ptr<State> state);
    void PopCurrent();
    void ProcessStateChange();
    std::unique_ptr<State> &GetCurrentState();
    bool IsEmpty() const;

private:
    std::stack<std::unique_ptr<State>> m_states;
    std::unique_ptr<State> m_newState;

    bool m_add = false;
    bool m_remove = false;
};
