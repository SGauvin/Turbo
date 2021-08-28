#pragma once

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Application/CompileTimeSettings.h"

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
        TextureTemplate(TextureType textureType);
        ~TextureTemplate();

        void bind() const;
    };

    using Texture = TextureTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLTexture.h"
