#pragma once

#include <entt/entt.hpp>
#include "Turbo/Core/Renderer/Abstraction/Shader.h"
#include "Turbo/Core/Application/CompileTimeSettings.h"
#include "Turbo/Core/Renderer/Abstraction/Texture.h"

namespace Turbo
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        
        void update();
        void draw(const glm::mat4& viewMatrix, const glm::vec3& cameraPosition, float aspectRatio, float lag = 1.0f);

        Entity createEntity();
        void deleteEntity(const Entity& entity);

        std::vector<Entity> loadGlTF(const std::string& path);

    private:
        entt::registry m_registry;
        Turbo::Shader<Turbo::renderingApi> m_shader;
        glm::vec3 m_lightPos = glm::vec3(20.f, 10.2f, 40.f);
        Texture m_texture;

        friend class Entity;
    };
} // Turbo
