#ifndef INCLUDED_TURBO_APPLICATION_H
#define INCLUDED_TURBO_APPLICATION_H

#include <chrono>
#include <numeric>
#include <vector>
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/Shader.h"
#include "Turbo/Core/Window/Window.h"

namespace Turbo
{
    class State;

    static constexpr RenderingApi renderingApi = RenderingApi::OpenGL;

    class Application
    {
    public:
        Application(const WindowAttributes& windowAttributes);
        ~Application();

        void push(State* state);
        void pop();

        void setTargetUps(double ups);
        void setTargetFps(double fps);
        void start();

        Window<renderingApi>& getWindow() { return m_window; }
        InputManager& getInputManager() { return m_inputManager; }

    private:
        InputManager m_inputManager;
        Window<renderingApi> m_window;

        std::vector<State*> m_states;

        Shader<renderingApi> m_shader;
        
        std::unique_ptr<VertexBuffer<renderingApi>> m_vertexBuffer;
        std::unique_ptr<IndexBuffer<renderingApi>> m_indexBuffer;
        std::uint32_t m_vertexArray;
        std::unique_ptr<VertexBuffer<renderingApi>> m_vertexBuffer2;
        std::unique_ptr<IndexBuffer<renderingApi>> m_indexBuffer2;
        std::uint32_t m_vertexArray2;


        std::chrono::duration<double> m_timePerUpdate = std::chrono::nanoseconds(1000000000 / 60);
        std::chrono::duration<double> m_timePerDraw = std::chrono::nanoseconds(1000000000 / 144);
        std::chrono::duration<double> m_updateLag{0};
        std::chrono::duration<double> m_drawLag{0};
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_APPLICATION_H
