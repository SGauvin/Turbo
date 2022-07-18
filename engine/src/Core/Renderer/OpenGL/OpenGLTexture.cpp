#include "Turbo/Core/Renderer/OpenGL/OpenGLTexture.h"
#include <stb_image.h>
#include <glad/glad.h>
#include "Turbo/Core/Log.h"

namespace Turbo
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init) -- m_texture is being assigned with glCreateBuffers
    TextureTemplate<RenderingApi::OpenGL>::TextureTemplate(TextureType textureType)
        : m_textureType(textureType)
    {
        glGenTextures(1, &m_texture);
    }

    TextureTemplate<RenderingApi::OpenGL>::~TextureTemplate()
    {
        glDeleteTextures(1, &m_texture);
    }

    void TextureTemplate<RenderingApi::OpenGL>::bind() const
    {
        glActiveTexture(GL_TEXTURE0 + static_cast<std::uint32_t>(m_textureType));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    void TextureTemplate<RenderingApi::OpenGL>::load(const std::string& file)
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // NOLINTNEXTLINE
        std::int32_t width, height, channelCount;
        std::uint8_t* data = stbi_load(file.c_str(), &width, &height, &channelCount, 0);

        if (data != nullptr)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            TURBO_ENGINE_ERROR("Couldn't load texture {}.", file);
        }

        stbi_image_free(data);
    }

    void TextureTemplate<RenderingApi::OpenGL>::load(const tinygltf::Image& image)
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const GLenum format = image.component == 1 ? GL_RED : image.component == 2 ? GL_RG : image.component == 3 ? GL_RGB : GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, format, GL_UNSIGNED_BYTE, image.image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
} // namespace Turbo
