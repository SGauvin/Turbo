#ifndef INCLUDED_TURBO_SCENE_H
#define INCLUDED_TURBO_SCENE_H

#include <entt/entt.hpp>
#include "Turbo/Core/Renderer/Abstraction/Shader.h"
#include "Turbo/Core/Settings.h"
#include "Turbo/Core/Renderer/Abstraction/Texture.h"

namespace Turbo
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        
        void update();
        void draw(const glm::mat4& viewMatrix, const glm::vec3& cameraPosition, float lag = 1.0f);

        Entity createEntity();
        void deleteEntity(const Entity& entity);

    private:
        entt::registry m_registry;
        Turbo::Shader<Turbo::renderingApi> m_shader;
        glm::vec3 m_lightPos = glm::vec3(2.f, 1.2f, 4.f);
        Texture m_texture;

        friend class Entity;
    };
} // Turbo

#endif // INCLUDED_TURBO_SCENE_H
