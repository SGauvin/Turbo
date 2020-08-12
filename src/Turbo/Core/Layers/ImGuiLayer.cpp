#include "Turbo/Core/Layers/ImGuiLayer.h"
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/imgui_impl_opengl3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"

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
        ImGui_ImplGlfw_InitForOpenGL(application.getWindow().getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::onAttach()
    {

    }

    void ImGuiLayer::onDetach()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::handleInput() {}

    void ImGuiLayer::update() {}

    void ImGuiLayer::draw(float lag)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = { m_window.getSize().x, m_window.getSize().y };
        io.DeltaTime = (1.0f / 60.0f);

        if (io.ConfigFlags | ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }

} // namespace Turbo