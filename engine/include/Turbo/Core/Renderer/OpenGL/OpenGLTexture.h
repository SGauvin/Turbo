#ifndef INCLUDED_TURBO_OPENGLTEXTURE_H
#define INCLUDED_TURBO_OPENGLTEXTURE_H

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

#endif // INCLUDED_TURBO_OPENGLTEXTURE_H
