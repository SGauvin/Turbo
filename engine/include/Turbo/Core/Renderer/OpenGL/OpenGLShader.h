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
        ~Shader();

        bool loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
        bool load(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        void bind() const;

        void setFloat3(const std::string& name, const glm::vec3& value);
        void setMatrix4(const std::string& name, const glm::mat4x4& value);

    private:
        std::uint32_t m_programId;
    };
} // namespace Turbo

#endif // INCLUDED_TURBO_OPENGLSHADER_H
