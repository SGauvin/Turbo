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

        m_inputContext->bindKeyPressEvents([](const Keyboard::KeyEvent& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.KeysDown[static_cast<std::uint16_t>(event.key)] = true;

            io.KeyCtrl = event.modifiers & Keyboard::Modifier::Control;
            io.KeyAlt = event.modifiers & Keyboard::Modifier::Alt;
            io.KeyShift = event.modifiers & Keyboard::Modifier::Shift;
            io.KeySuper = event.modifiers & Keyboard::Modifier::Super;

            return false;
        });

        m_inputContext->bindKeyReleaseEvents([](const Keyboard::KeyEvent& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.KeysDown[static_cast<std::uint16_t>(event.key)] = false;

            io.KeyCtrl = event.modifiers & Keyboard::Modifier::Control;
            io.KeyAlt = event.modifiers & Keyboard::Modifier::Alt;
            io.KeyShift = event.modifiers & Keyboard::Modifier::Shift;
            io.KeySuper = event.modifiers & Keyboard::Modifier::Super;

            return false;
        });

        m_inputContext->bindTextEnterEvents([](std::uint32_t character) {
            ImGuiIO& io = ImGui::GetIO();
            io.AddInputCharacter(character);
            return false;
        });

        static std::uint8_t counter = 0;
        static InputHandle handle = m_inputContext->bindMouseMoveEvents([](const Mouse::MoveEvent& event) {
            TURBO_ENGINE_INFO("Counter: {}", ++counter);

            static InputHandle handle2 = handle;

            if (counter == 10)
            {
                handle.unbind();
                handle2.unbind();
            }
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos = {static_cast<float>(event.mousePosition.x), static_cast<float>(event.mousePosition.y)};
            return false;
        });

        m_inputContext->bindMouseMoveEvents([](const Mouse::MoveEvent& event) {
            TURBO_ENGINE_INFO("x: {}, y: {}", event.mousePosition.x, event.mousePosition.y);
            return false;
        });

        m_inputContext->bindMousePressEvents([](const Mouse::ButtonEvent& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.MouseDown[static_cast<std::uint8_t>(event.button)] = true;
            return false;
        });

        m_inputContext->bindMouseReleaseEvents([](const Mouse::ButtonEvent& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.MouseDown[static_cast<std::uint8_t>(event.button)] = false;
            return false;
        });

        m_inputContext->bindMouseScrollEvents([](const Mouse::ScrollEvent& event) {
            ImGuiIO& io = ImGui::GetIO();
            io.MouseWheel += static_cast<float>(event.scrollDelta.y);
            io.MouseWheelH += static_cast<float>(event.scrollDelta.x);
            return false;
        });
    }

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::handleInput() {}

    void ImGuiLayer::update() {}

    void ImGuiLayer::draw(float lag)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = 1.0f / 144.0f;
        io.DisplaySize = {static_cast<float>(m_window.getSize().x), static_cast<float>(m_window.getSize().y)};

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

} // namespace Turbo