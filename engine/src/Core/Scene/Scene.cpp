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
        m_texture.load("../assets/wood.png");
    }
        
    void Scene::update()
    {}

    void Scene::draw(const glm::mat4& viewMatrix, const glm::vec3& cameraPosition, float aspectRatio, float lag)
    {
        auto view = m_registry.view<TransformComponent, MeshComponent>();
        m_shader.bind();
        m_texture.bind();
        m_shader.setFloat3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
        m_shader.setFloat3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
        m_shader.setFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        m_shader.setFloat("material.shininess", 32.0f);

        m_shader.setFloat3("light.position", m_lightPos);
        m_shader.setFloat3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
        m_shader.setFloat3("light.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f));
        m_shader.setFloat3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f)); 

        for (auto entity : view)
        {
            auto [transform, mesh] = view.get(entity);
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

            m_shader.setMatrix4("model", transform.getTransform());
            m_shader.setMatrix4("view", viewMatrix);
            m_shader.setMatrix4("projection", projection);
            m_shader.setFloat3("cameraPosition", cameraPosition);
            

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