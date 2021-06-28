#ifndef INCLUDED_TURBO_CALLABLE_H
#define INCLUDED_TURBO_CALLABLE_H

#include <functional>

namespace Turbo
{
    template<typename R, typename... Args>
    class Callable
    {
    public:
        Callable() {}

        template<typename O, typename F>
        Callable(O* o, F f)
            : m_callback([o, f](Args&&... args) { (o->*f)(std::forward<Args>(args)...); })
        {
        }

        template<typename F>
        Callable(F f)
            : m_callback(f)
        {
        }

        // R operator()(Args&&... args) { return m_callback(std::forward<Args>(args)...); }
        R operator()(const Args&... args) const { return m_callback(args...); }

        Callable* clone() { return new Callable(*this); }

    private:
        std::function<R(Args...)> m_callback;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_CALLABLE_H
