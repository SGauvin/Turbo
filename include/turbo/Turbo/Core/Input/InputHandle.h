#ifndef INCLUDED_TURBO_INPUTHANDLE_H
#define INCLUDED_TURBO_INPUTHANDLE_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include "Turbo/Core/Log.h"

namespace Turbo
{
    class InputContext;

    class InputHandle
    {
    private:
        friend class InputContext;

        class InputHandleHolder
        {
        private:
            friend class InputHandle;

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
        InputHandle()
            : m_inputContext(nullptr)
            , m_isBound(false)
        {
        }

        template<typename T>
        InputHandle(T& vector, InputContext* inputContext, std::uint32_t id)
            : m_inputContext(inputContext)
            , m_isBound(true)
        {
            static_assert(sizeof(InputHandleVector<T>) <= s_maxSize);
            new (m_buffer) InputHandleVector<T>(vector, id);
        }

        InputHandle(const InputHandle& other)
            : m_inputContext(other.m_inputContext)
            , m_isBound(true)
        {
            if (this == &other)
            {
                return;
            }

            for (std::size_t i = 0; i < s_maxSize; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }

            m_isBound = other.m_isBound;
        }

        InputHandle& operator=(const InputHandle& other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (reinterpret_cast<const InputHandleHolder*>(m_buffer)->m_id != reinterpret_cast<const InputHandleHolder*>(other.m_buffer)->m_id &&
                m_isBound == true)
            {
                TURBO_WARNING("Warning, InputHandle should be unbound before bound to another input.");
            }

            m_inputContext = other.m_inputContext;

            for (std::size_t i = 0; i < s_maxSize; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }

            m_isBound = other.m_isBound;
            return *this;
        }

        InputHandle(const InputHandle&& other)
        {
            m_inputContext = other.m_inputContext;

            for (std::size_t i = 0; i < s_maxSize; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }

            m_isBound = other.m_isBound;
        }

        InputHandle& operator=(const InputHandle&& other)
        {
            m_inputContext = other.m_inputContext;

            if (reinterpret_cast<const InputHandleHolder*>(m_buffer)->m_id != reinterpret_cast<const InputHandleHolder*>(other.m_buffer)->m_id &&
                m_isBound == true)
            {
                TURBO_WARNING("Warning, InputHandle should be unbound before bound to another input.");
            }

            for (std::size_t i = 0; i < s_maxSize; i++)
            {
                m_buffer[i] = other.m_buffer[i];
            }

            m_isBound = other.m_isBound;
            return *this;
        }

        ~InputHandle()
        {
            if (m_inputContext != nullptr)
            {
                reinterpret_cast<InputHandleHolder*>(m_buffer)->~InputHandleHolder();
            }
        }

        void unbind();
        bool isBound() { return m_isBound; }

        std::string a = "";

    protected:
        const static std::uint8_t s_maxSize = 24;
        std::byte m_buffer[s_maxSize];
        InputContext* m_inputContext = nullptr;
        bool m_isBound = false;

    private:
        void removeFromVector() { reinterpret_cast<InputHandleHolder*>(m_buffer)->unbind(); }
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_INPUTHANDLE_H