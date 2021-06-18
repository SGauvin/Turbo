#ifndef INCLUDED_TURBO_SHADER_H
#define INCLUDED_TURBO_SHADER_H

#include "Turbo/Core/Renderer/RenderingApi.h"

namespace Turbo
{
    template<RenderingApi>
    class Shader
    {
    public:
        Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        ~Shader();
        
        bool load(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
        void bind() const;
        void unbind() const;
    };

} // namespace Turbo

#endif //INCLUDED_TURBO_SHADER_H