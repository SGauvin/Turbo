#ifndef INCLUDED_TURBO_IMGGUILAYER_H
#define INCLUDED_TURBO_IMGGUILAYER_H

#include "Turbo/Core/Layers/Layer.h"
#include "Turbo/Core/Input/InputContext.h"

namespace Turbo
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer(Application& application);
        ~ImGuiLayer();

        void handleInput() override;
        void update() override;
        void draw(float lag = 1.0) override;

    private:
        InputContext* m_inputContext = nullptr;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_IMGGUILAYER_H