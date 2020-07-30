#ifndef INCLUDED_TURBO_CALLABLE_H
#define INCLUDED_TURBO_CALLABLE_H

namespace Turbo
{
    template<typename ReturnType, typename... Args>
    class Callable
    {
    public:
        virtual ~Callable() = default;

        virtual ReturnType operator()(const Args&... args) = 0;

        virtual Callable<ReturnType, Args...>* clone() = 0;
    };

    template<typename ReturnType, typename F, typename... Args>
    class Functor : public Callable<ReturnType, Args...>
    {
    public:
        explicit Functor(F function)
            : m_function(function)
        {
        }

        virtual ReturnType operator()(const Args&... args) override { return m_function(args...); }

        virtual Callable<ReturnType, Args...>* clone() { return new Functor<ReturnType, F, Args...>(m_function); }

    private:
        F m_function;
    };

    template<typename ReturnType, typename O, typename F, typename... Args>
    class Method : public Callable<ReturnType, Args...>
    {
    public:
        Method(O* object, F method)
            : m_object(object)
            , m_method(method)
        {
        }

        virtual ReturnType operator()(const Args&... args) override { return (m_object->*m_method)(args...); }

        virtual Callable<ReturnType, Args...>* clone() { return new Method<ReturnType, O, F, Args...>(m_object, m_method); }

    private:
        O* m_object;
        F m_method;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_CALLABLE_H
