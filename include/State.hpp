#pragma once

class Context;

class State
{
public:
    State(Context *context) : m_context(context){};
    virtual ~State() = default;

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void ProcessInputs() = 0;

    virtual void Pause() { m_paused = true; }
    virtual void Start() { m_paused = false; }

    bool IsPaused() const { return m_paused; }

protected:
    Context *m_context = nullptr;
    bool m_paused = false;
};
