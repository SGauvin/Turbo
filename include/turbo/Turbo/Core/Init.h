#ifndef INCLUDED_TURBO_INIT_H
#define INCLUDED_TURBO_INIT_H

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Window.h"

namespace Turbo
{
    void init() { Window::init(); }
} // namespace Turbo

#endif // INCLUDED_TURBO_INIT_H