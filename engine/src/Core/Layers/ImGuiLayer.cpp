#include "Turbo/Core/Layers/ImGuiLayer.h"
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Window/OpenGLWindow.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

namespace Turbo
{
    ImGuiLayer::ImGuiLayer(Application& application)
        : Layer(application)
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

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::onAttach()
    {
    }

    void ImGuiLayer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::handleInput()
    {
    }

    void ImGuiLayer::update()
    {
    }

    void ImGuiLayer::draw(float /*lag*/)
    {
    }

} // namespace Turbo
