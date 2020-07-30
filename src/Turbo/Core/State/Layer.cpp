#include "Turbo/Core/State/Layer.h"

namespace Turbo
{
    Layer::Layer(Application& application)
        : m_application(application)
        , m_inputManager(application.getInputManager())
        , m_window(application.getWindow())
    {
    }
}