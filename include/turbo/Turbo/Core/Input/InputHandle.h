#ifndef INCLUDED_TURBO_INPUTHANDLE_H
#define INCLUDED_TURBO_INPUTHANDLE_H

#include <cstddef>
#include <cstdint>
#include <algorithm>

namespace Turbo
{
    class InputContext;

    class InputHandle
    {
    private:
        friend class InputContext;

        class InputHandleHolder
        {
        public:
            InputHandleHolder(std::uint32_t id)
                : m_id(id)
            {
            }

            InputHandleHolder() = default;
            virtual ~InputHandleHolder() {}
            InputHandleHolder(const InputHandleHolder& other) = default;
            InputHandleHolder(const InputHandleHolder&& other) = delete;
            const InputHandleHolder& operator=(const InputHandleHolder& other) = delete;
            const InputHandleHolder& operator=(const InputHandleHolder&& other) = delete;

            virtual void unbind() = 0;

        protected:
            std::uint32_t m_id;
        };

        template<typename T>
        class InputHandleVector : public InputHandleHolder
        {
        public:
            InputHandleVector(T& vector, std::uint32_t id)
                : InputHandleHolder(id)
                , m_vector(vector)
            {
            }

            void unbind() override
            {
                for (auto it = m_vector.begin(); it != m_vector.end(); ++it)
                {
                    if (it->first == m_id)
                    {
                        m_vector.erase(it);
                        break;
                    }
                }
            }

        private:
            T& m_vector;
        };

    public:
        template<typename T>
        InputHandle(T& vector, InputContext& inputContext, std::uint32_t id)
            : m_inputContext(inputContext)
        {
            static_assert(sizeof(InputHandleVector<T>) <= s_maxSize);
            new (m_buffer) InputHandleVector<T>(vector, id);
        }

        InputHandle(const InputHandle& other)
            : m_inputContext(other.m_inputContext)
        {
            if (this == &other)
            {
                return;
            }

            for (std::size_t i = 0; i < s_maxSize; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }
        }

        ~InputHandle() { reinterpret_cast<InputHandleHolder*>(m_buffer)->~InputHandleHolder(); }

        void unbind();

    protected:
        const static std::uint8_t s_maxSize = 24;
        std::byte m_buffer[s_maxSize];
        InputContext& m_inputContext;

    private:
        void removeFromVector() { reinterpret_cast<InputHandleHolder*>(m_buffer)->unbind(); }
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_INPUTHANDLE_H