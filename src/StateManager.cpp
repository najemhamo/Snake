#include "StateManager.hpp"
#include "State.hpp"

void StateManager::Add(std::unique_ptr<State> state)
{
    m_add = true;
    m_newState = std::move(state);
}

void StateManager::PopCurrent()
{
    m_remove = true;
}

void StateManager::ProcessStateChange()
{
    if (m_remove && (!m_states.empty()))
    {
        m_states.pop();

        if (!m_states.empty())
        {
            m_states.top()->Start();
        }

        m_remove = false;
    }

    if (m_add)
    {
        if (!m_states.empty())
        {
            m_states.pop();
        }

        m_states.push(std::move(m_newState));
        m_states.top()->Init();
        m_states.top()->Start();
        m_add = false;
    }
}

std::unique_ptr<State> &StateManager::GetCurrentState()
{
    return m_states.top();
}

bool StateManager::IsEmpty() const
{
    return m_states.empty();
}
