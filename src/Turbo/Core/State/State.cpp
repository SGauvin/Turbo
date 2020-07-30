#include "Turbo/Core/State/State.h"
#include <algorithm>

namespace Turbo
{
    State::State(Application& application)
        : m_application(application)
        , m_inputManager(application.getInputManager())
        , m_window(application.getWindow())
    {
    }

    State::~State()
    {
        for (const auto& layer : m_layers)
        {
            delete layer;
        }
    }

    void State::pushLayer(Layer* layer)
    {
        m_layers.push_back(layer);
    }

    void State::removeLayer(Layer* layer)
    {
        auto it = std::find(m_layers.cbegin(), m_layers.cend(), layer);
        if (it != m_layers.cend())
        {
            delete *it;
            m_layers.erase(it);
        }
    }

    void State::popLayer()
    {
        if (!m_layers.empty())
        {
            delete m_layers.back();
            m_layers.pop_back();
        }
    }

} // namespace Turbo