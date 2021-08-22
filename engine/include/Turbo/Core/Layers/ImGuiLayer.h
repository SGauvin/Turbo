#pragma once

#include "Turbo/Core/Input/InputContext.h"
#include "Turbo/Core/Layers/Layer.h"

namespace Turbo
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer(Application& application);
        ~ImGuiLayer();

        void onAttach() override;
        void onDetach() override;

        void handleInput() override;
        void update() override;
        void draw(float lag = 1.0) override;

    private:
        InputContext* m_inputContext = nullptr;
    };
} // namespace Turbo
