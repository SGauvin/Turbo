#ifndef INCLUDED_TURBO_SHADER_H
#define INCLUDED_TURBO_SHADER_H

#include <glm/glm.hpp>

#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class Shader
    {
    public:
        Shader();
        ~Shader();

        bool loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
        bool load(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        void bind() const;

        void setMatrix4(const std::string& name, const glm::mat4x4& value);
    };

} // namespace Turbo

#include "Turbo/Core/Renderer/OpenGL/OpenGLShader.h"

#endif // INCLUDED_TURBO_SHADER_H
