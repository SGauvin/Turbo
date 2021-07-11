#include "Turbo/Core/Scene/Components/TransformComponent.h"
#include "Turbo/Core/Scene/Components/MeshComponent.h"
#include "Turbo/Core/Scene/Entity.h"
#include "Turbo/Core/Scene/Scene.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"

namespace Turbo
{
    Scene::Scene()
    {
        m_shader.loadFromFile("../assets/shader.vert", "../assets/shader.frag");
    }
        
    void Scene::update()
    {}

    void Scene::draw(const glm::mat4& viewMatrix, const glm::vec3& cameraPosition, float lag)
    {
        auto view = m_registry.view<TransformComponent, MeshComponent>();
        m_shader.bind();

        for (auto entity : view)
        {
            auto [transform, mesh] = view.get(entity);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.f / 9.f, 0.1f, 100.0f);

            m_shader.setMatrix4("model", transform.getTransform());
            m_shader.setMatrix4("view", viewMatrix);
            m_shader.setMatrix4("projection", projection);
            m_shader.setFloat3("cameraPosition", cameraPosition);
            m_shader.setFloat3("lightPosition", m_lightPos);

            Turbo::RenderCommand::draw<Turbo::renderingApi>(mesh.getVertexArray());
        }
    }

    Entity Scene::createEntity()
    {
        return Entity(m_registry.create(), this);
    }
    
    void Scene::deleteEntity(const Entity& entity)
    {
        m_registry.destroy(entity);
    }
} // namespace Turbo