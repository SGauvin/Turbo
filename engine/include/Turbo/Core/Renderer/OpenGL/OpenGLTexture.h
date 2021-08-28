#pragma once

#include <string>
#include <tiny_gltf.h>

#include "Turbo/Core/Renderer/Abstraction/Texture.h"
#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<>
    class TextureTemplate<RenderingApi::OpenGL>
    {
    public:
        TextureTemplate(TextureType textureType);
        ~TextureTemplate();

        void bind() const;

        void load(const std::string& file);
        void load(const tinygltf::Image& image);

    private:
        std::uint32_t m_texture;
        TextureType m_textureType;
    };
} // namespace Turbo
