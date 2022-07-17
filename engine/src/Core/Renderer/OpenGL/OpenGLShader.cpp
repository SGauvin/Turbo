#include "Turbo/Core/Renderer/OpenGL/OpenGLShader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Turbo/Core/Log.h"

namespace Turbo
{
    Shader<RenderingApi::OpenGL>::~Shader()
    {
        glDeleteProgram(m_programId);
    }

    bool Shader<RenderingApi::OpenGL>::loadFromFile(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
    {
        auto loadFile = [](const std::string& filepath, std::string& out) -> void
        {
            std::ifstream file(filepath);

            TURBO_ASSERT(file, "Couldn't load shader");

            std::stringstream shaderStream;
            shaderStream << file.rdbuf();
            out = shaderStream.str();
        };

        std::string vertexCode;
        loadFile(vertexShaderFile, vertexCode);

        std::string fragmentCode;
        loadFile(fragmentShaderFile, fragmentCode);

        return load(vertexCode, fragmentCode);
    }

    bool Shader<RenderingApi::OpenGL>::load(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
    {
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const auto* source = (const GLchar*)vertexShaderSource.c_str();
        glShaderSource(vertexShader, 1, &source, nullptr);

        // Compile the vertex shader
        glCompileShader(vertexShader);

        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            TURBO_ENGINE_ERROR("Vertex shader compilation failed.");
            TURBO_ENGINE_ERROR("{}", infoLog.data());
            return false;
        }

        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = (const GLchar*)fragmentShaderSource.c_str();
        glShaderSource(fragmentShader, 1, &source, nullptr);

        // Compile the fragment shader
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);

            // Use the infoLog as you see fit.
            TURBO_ENGINE_ERROR("Fragment shader compilation failed.");
            TURBO_ENGINE_ERROR("{}", infoLog.data());
            return false;
        }

        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_programId = glCreateProgram();

        // Attach our shaders to our program
        glAttachShader(m_programId, vertexShader);
        glAttachShader(m_programId, fragmentShader);

        // Link our program
        glLinkProgram(m_programId);

        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(m_programId, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_programId, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(m_programId);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            // Use the infoLog as you see fit.
            TURBO_ENGINE_ERROR("Shader linkage failed.");
            TURBO_ENGINE_ERROR("{}", infoLog.data());
            return false;
        }

        // Always detach shaders after a successful link.
        glDetachShader(m_programId, vertexShader);
        glDetachShader(m_programId, fragmentShader);
        return true;
    }

    void Shader<RenderingApi::OpenGL>::bind() const
    {
        glUseProgram(m_programId);
    }

    void Shader<RenderingApi::OpenGL>::setInt(const std::string& name, std::int32_t value)
    {
        const auto location = glGetUniformLocation(m_programId, name.c_str());
        TURBO_ASSERT((location != -1), "Uniform {} location not found", name);
        glUniform1iv(location, 1, &value);
    }

    void Shader<RenderingApi::OpenGL>::setFloat(const std::string& name, float value)
    {
        const auto location = glGetUniformLocation(m_programId, name.c_str());
        TURBO_ASSERT((location != -1), "Uniform {} location not found", name);
        glUniform1fv(location, 1, &value);
    }

    void Shader<RenderingApi::OpenGL>::setFloat3(const std::string& name, const glm::vec3& value)
    {
        const auto location = glGetUniformLocation(m_programId, name.c_str());
        TURBO_ASSERT((location != -1), "Uniform {} location not found", name);
        glUniform3fv(location, 1, glm::value_ptr(value));
    }

    void Shader<RenderingApi::OpenGL>::setMatrix4(const std::string& name, const glm::mat4x4& value)
    {
        const auto location = glGetUniformLocation(m_programId, name.c_str());
        TURBO_ASSERT((location != -1), "Uniform {} location not found", name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
} // namespace Turbo
