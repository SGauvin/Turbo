#include "Turbo/Core/States/State.h"
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
            layer->onDetach();
            delete layer;
        }
    }

    void State::pushLayer(Layer* layer)
    {
        m_layers.push_back(layer);
        layer->onAttach();
    }

    void State::removeLayer(Layer* layer)
    {
        auto it = std::find(m_layers.cbegin(), m_layers.cend(), layer);
        if (it != m_layers.cend())
        {
            m_layers.erase(it);
            layer->onDetach();
            delete *it;
        }
    }

    void State::popLayer()
    {
        if (!m_layers.empty())
        {
            m_layers.back()->onDetach();
            delete m_layers.back();
            m_layers.pop_back();
        }
    }

} // namespace Turbo
