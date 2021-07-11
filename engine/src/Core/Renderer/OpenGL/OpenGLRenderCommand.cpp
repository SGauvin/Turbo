#include <glad/glad.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"
#include "Turbo/Core/Log.h"


namespace
{
    void openGLMessageCallback(unsigned source, unsigned type, unsigned id, unsigned severity, int length, const char* message, const void* userParam)
	{
		switch (severity)
		{
        case GL_DEBUG_SEVERITY_HIGH:
            TURBO_ENGINE_ERROR(message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            TURBO_ENGINE_ERROR(message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            TURBO_ENGINE_WARNING(message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            TURBO_ENGINE_INFO(message);
            break;
		}	
	}
} // namepsace

namespace Turbo
{
    namespace RenderCommand
    {
        template<>
        void init<RenderingApi::OpenGL>()
        {
#if !defined(NDEBUG)
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openGLMessageCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // NDEBUG
            glEnable(GL_DEPTH_TEST);
        }

        template<>
        void setViewport<RenderingApi::OpenGL>(const glm::uvec2& position, const glm::uvec2& size)
        {
            glViewport(position.x, position.y, size.x, size.y);
        }

        template<>
        void setClearColor<RenderingApi::OpenGL>(const glm::vec4& color)
        {
            glClearColor(color.r, color.g, color.b, color.a);
        }

        template<>
        void clear<RenderingApi::OpenGL>()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        template<>
        void draw<RenderingApi::OpenGL>(VertexArray* vertexArray)
        {
            glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
        }

        template<>
        void beginViewport<RenderingApi::OpenGL>(FrameBuffer<RenderingApi::OpenGL>* frameBuffer)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // VIEWPORT

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
            ImGui::Begin("Viewport");
            static ImVec2 viewportSize = ImVec2(0, 0);
            ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
            if (viewportSize.x != currentViewportSize.x || viewportSize.y != currentViewportSize.y)
            {
                viewportSize = currentViewportSize;
                frameBuffer->resize(glm::ivec2(viewportSize.x, viewportSize.y));
            }

            frameBuffer->bind();
            
            setViewport<RenderingApi::OpenGL>({0, 0}, {static_cast<std::int32_t>(viewportSize.x), static_cast<std::int32_t>(viewportSize.y)});
        }

        template<>
        void endViewport<RenderingApi::OpenGL>(FrameBuffer<RenderingApi::OpenGL>* frameBuffer, glm::uvec2 windowSize)
        {
            frameBuffer->unbind();

            ImGui::Image(reinterpret_cast<void*>(frameBuffer->getTexture()), ImVec2(frameBuffer->getSize().x, frameBuffer->getSize().y), ImVec2(0, 1), ImVec2(1, 0));
            ImGui::End();
            ImGui::PopStyleVar();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)};
            io.DeltaTime = (1.0f / 60.0f);

            if (io.ConfigFlags | ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backupCurrentContext);
            }
        }
    } // namspace RenderCommand
} // namespace Turbo