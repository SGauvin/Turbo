#pragma once

#include <string>

#include "Turbo/Core/Renderer/Abstraction/Texture.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<>
    class TextureTemplate<RenderingApi::OpenGL>
    {
    public:
        TextureTemplate();
        ~TextureTemplate();

        void bind() const;
        void load(const std::string& file);

    private:
        std::uint32_t m_texture;
    };
} // namespace Turbo
