#ifndef INCLUDED_TURBO_OPENGLSHADER_H
#define INCLUDED_TURBO_OPENGLSHADER_H

#include <string>

class OpenGLShader
{
public:
    OpenGLShader() {}
    OpenGLShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    ~OpenGLShader();

    bool load(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    void bind() const;
    void unbind() const;

private:
    std::uint32_t m_programId;
};

#endif // INCLUDED_TURBO_OPENGLSHADER_H
