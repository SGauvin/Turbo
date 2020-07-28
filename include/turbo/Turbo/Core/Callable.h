#ifndef TURBO_CALLABLE_H
#define TURBO_CALLABLE_H

namespace Turbo
{
    template<typename... Args>
    class Callable
    {
    public:
        virtual ~Callable() = default;

        virtual void operator()(const Args&... args) = 0;

        virtual Callable<Args...>* clone() = 0;
    };

    template<typename F, typename... Args>
    class Functor : public Callable<Args...>
    {
    public:
        explicit Functor(F function)
            : m_function(function)
        {
        }

        virtual void operator()(const Args&... args) override { m_function(args...); }

        virtual Callable<Args...>* clone() { return new Functor<F, Args...>(m_function); }

    private:
        F m_function;
    };

    template<typename O, typename F, typename... Args>
    class Method : public Callable<Args...>
    {
    public:
        Method(O* object, F method)
            : m_object(object)
            , m_method(method)
        {
        }

        virtual void operator()(const Args&... args) override { (m_object->*m_method)(args...); }

        virtual Callable<Args...>* clone() { return new Method<O, F, Args...>(m_object, m_method); }

    private:
        O* m_object;
        F m_method;
    };
} // namespace Turbo

#endif // TURBO_CALLABLE_H
