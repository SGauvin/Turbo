#pragma once

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Settings.h"

namespace Turbo
{
    enum class TextureType
    {
        Emissive = 0,
    };
    template<RenderingApi>
    class TextureTemplate
    {
    public:
        TextureTemplate();
        ~TextureTemplate();

        void bind() const;
        void setTextureType(TextureType textureType);
    };

    using Texture = TextureTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLTexture.h"
