#include "Turbo/Core/Application/EditorApplicationModeHandler.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/imgui.h>
#include "Turbo/Core/Application/Application.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"

namespace Turbo
{
    ApplicationModeHandlerImpl<ApplicationMode::Editor>::ApplicationModeHandlerImpl(Application& application)
        : m_application(application)
        , m_frameBuffer(glm::ivec2(1280, 720))
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad | ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
        ImGui::StyleColorsDark();
        auto& window = application.getWindow();
        auto handle = window.getHandle();
        ImGui_ImplGlfw_InitForOpenGL(handle, true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    ApplicationModeHandlerImpl<ApplicationMode::Editor>::~ApplicationModeHandlerImpl()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ApplicationModeHandlerImpl<ApplicationMode::Editor>::begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ApplicationModeHandlerImpl<ApplicationMode::Editor>::end()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO& io = ImGui::GetIO();
        const glm::uvec2 windowSize = m_application.getWindow().getSize();
        io.DisplaySize = {static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)};

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ApplicationModeHandlerImpl<ApplicationMode::Editor>::beginViewport()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});
        ImGui::Begin("Viewport");
        ImVec2 currentViewportSize = ImGui::GetContentRegionAvail();
        if (m_oldViewportSize.x != static_cast<std::uint32_t>(currentViewportSize.x) ||
            m_oldViewportSize.y != static_cast<std::uint32_t>(currentViewportSize.y))
        {
            m_frameBuffer.resize(glm::ivec2(currentViewportSize.x, currentViewportSize.y));
            m_oldViewportSize = {currentViewportSize.x, currentViewportSize.y};
        }

        m_frameBuffer.bind();

        RenderCommand::setViewport({0, 0}, {static_cast<std::int32_t>(currentViewportSize.x), static_cast<std::int32_t>(currentViewportSize.y)});
    }

    void ApplicationModeHandlerImpl<ApplicationMode::Editor>::endViewport()
    {
        m_frameBuffer.unbind();

        ImGui::Image(reinterpret_cast<void*>(m_frameBuffer.getTexture()),
                     ImVec2(static_cast<float>(m_frameBuffer.getSize().x), static_cast<float>(m_frameBuffer.getSize().y)),
                     ImVec2(0, 1),
                     ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();
    }

    glm::ivec2 ApplicationModeHandlerImpl<ApplicationMode::Editor>::getViewportSize() const
    {
        return m_frameBuffer.getSize();
    }
} // namespace Turbo
