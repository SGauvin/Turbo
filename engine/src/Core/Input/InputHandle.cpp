#include <Turbo/Core/Input/InputContext.h>
#include <Turbo/Core/Input/InputHandle.h>

namespace Turbo
{
    void InputHandle::unbind()
    {
        if (m_inputContext != nullptr)
        {
            m_inputContext->unbind(this);
        }
        m_isBound = false;
    }
} // namespace Turbo