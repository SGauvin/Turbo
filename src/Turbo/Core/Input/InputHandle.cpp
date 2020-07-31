#include <Turbo/Core/Input/InputHandle.h>
#include <Turbo/Core/Input/InputContext.h>

namespace Turbo
{
    void InputHandle::unbind()
    {
        m_inputContext.unbind(this);
    }
}