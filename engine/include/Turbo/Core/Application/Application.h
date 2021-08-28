#pragma once

#include <chrono>
#include <numeric>
#include <vector>
#include "Turbo/Core/Renderer/Abstraction/FrameBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/Shader.h"
#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Window/Window.h"

namespace Turbo
{
    class Application
    {
    public:
        Application(const WindowAttributes& windowAttributes);
        ~Application();

        template<typename T, typename... Args>
        T* push(Args&... args)
        {
            static_assert(std::is_base_of<State, T>::value);
            std::uint8_t* alloc = new std::uint8_t[sizeof(T)];
            m_inputManager.onStateChange(reinterpret_cast<State*>(alloc));
            T* state = new(alloc) T(args...);
            m_states.push_back(state);
            onStatePushed();
            return state;
        }

        void pop();

        void setTargetUps(double ups);
        void setTargetFps(double fps);
        void start();

        glm::uvec2 getViewportSize() const;

        Window<renderingApi>& getWindow() { return m_window; }
        InputManager& getInputManager() { return m_inputManager; }

    private:
        InputManager m_inputManager;
        Window<renderingApi> m_window;

        std::unique_ptr<Turbo::FrameBuffer<Turbo::renderingApi>> m_viewportFrameBuffer;

        std::vector<State*> m_states;

        std::chrono::duration<double> m_timePerUpdate = std::chrono::nanoseconds(1000000000 / 144);
        std::chrono::duration<double> m_timePerDraw = std::chrono::nanoseconds(1000000000 / 144);
        std::chrono::duration<double> m_updateLag{0};
        std::chrono::duration<double> m_drawLag{0};

        void onStatePushed();
    };
} // namespace Turbo
