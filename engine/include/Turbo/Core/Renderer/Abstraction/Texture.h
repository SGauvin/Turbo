#ifndef INCLUDED_TURBO_TEXTURE_H
#define INCLUDED_TURBO_TEXTURE_H

#include "Turbo/Core/Renderer/RenderingApi.h"
#include "Turbo/Core/Settings.h"

namespace Turbo
{
    template<RenderingApi>
    class TextureTemplate
    {
    public:
        TextureTemplate();
        ~TextureTemplate();

        void bind() const;
    };

    using Texture = TextureTemplate<renderingApi>;
} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLTexture.h"

#endif // INCLUDED_TURBO_TEXTURE_H
