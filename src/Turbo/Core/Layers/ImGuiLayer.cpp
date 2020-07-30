#include "Turbo/Core/Layers/ImGuiLayer.h"
#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/imgui_impl_opengl3.h"
#include "imgui/imgui.h"

namespace Turbo
{
    ImGuiLayer::ImGuiLayer(Application& application)
        : Layer(application)
    {
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos;

        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");

        m_inputContext = m_inputManager.createInputContext();

        m_inputContext->bindKeyPressEvents([](const Keyboard::Event& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.KeysDown[static_cast<std::uint16_t>(event.key)] = true;
            return false;
        });

        m_inputContext->bindKeyReleaseEvents([](const Keyboard::Event& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.KeysDown[static_cast<std::uint16_t>(event.key)] = false;
            return false;
        });

        // m_inputContext->bindMouseMoveEvent([](const glm::dvec2& mousePos) {
        //     return false;
        // });

        m_inputContext->bindMousePressEvents([](const Mouse::Event& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos = {event.position.x, event.position.y};
            io.MouseDown[static_cast<std::uint8_t>(event.button)] = true;
            return false;
        });

        m_inputContext->bindMouseReleaseEvents([](const Mouse::Event& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos = {event.position.x, event.position.y};
            io.MouseDown[static_cast<std::uint8_t>(event.button)] = false;
            return false;
        });
    }

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::handleInput() {}

    void ImGuiLayer::update() {}

    void ImGuiLayer::draw(float lag)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = 1.0f / 60.0f;
        io.DisplaySize = {static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y)};

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
} // namespace Turbo