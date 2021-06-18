#ifndef INCLUDED_TURBO_OPENGLSHADER_H
#define INCLUDED_TURBO_OPENGLSHADER_H

#include <string>
#include "Turbo/Core/Renderer/Abstraction/Shader.h"

namespace Turbo
{
    template<>
    class Shader<RenderingApi::OpenGL>
    {
    public:
        Shader() {}
        Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        ~Shader();

        bool load(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        void bind() const;
        void unbind() const;

    private:
        std::uint32_t m_programId;
    };
}

#endif // INCLUDED_TURBO_OPENGLSHADER_H
