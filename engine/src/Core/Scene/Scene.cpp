#include <glm/glm.hpp>
#include <algorithm>

#include "Turbo/Core/Scene/Components/TransformComponent.h"
#include "Turbo/Core/Scene/Components/MeshComponent.h"
#include "Turbo/Core/Scene/Entity.h"
#include "Turbo/Core/Scene/Scene.h"
#include "Turbo/Core/Scene/GlTFLoader.h"
#include "Turbo/Core/Renderer/Abstraction/RenderCommand.h"

namespace Turbo
{
    Scene::Scene()
    {
        m_shader.loadFromFile("../assets/shader.vert", "../assets/shader.frag");
        m_shader.bind();
        m_shader.setInt("texture1", static_cast<std::int32_t>(TextureType::Emissive));
    }
        
    void Scene::update()
    {}

    void Scene::draw(const glm::mat4& viewMatrix, const glm::vec3& cameraPosition, float aspectRatio, float lag)
    {
        auto view = m_registry.view<TransformComponent, MeshComponent>();
        m_shader.bind();
        m_shader.setFloat3("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
        m_shader.setFloat3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        m_shader.setFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        m_shader.setFloat("material.shininess", 256.0f);

        m_shader.setFloat3("light.position", m_lightPos);
        m_shader.setFloat3("light.ambient",  glm::vec3(0.2f, 0.2f, 0.2f));
        m_shader.setFloat3("light.diffuse",  glm::vec3(0.9f, 0.9f, 0.9f));
        m_shader.setFloat3("light.specular", glm::vec3(0.5f, 0.5f, 0.5f)); 

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 600.0f);
        for (auto entity : view)
        {
            auto [transform, mesh] = view.get(entity);

            if (mesh.m_texture != nullptr)
            {
                mesh.m_texture->bind();
            }
            m_shader.setMatrix4("model", transform.getTransform());
            m_shader.setMatrix4("view", viewMatrix);
            m_shader.setMatrix4("projection", projection);
            m_shader.setFloat3("cameraPosition", cameraPosition);
            
            Turbo::RenderCommand::draw(mesh.m_vertexArray.get());
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

    std::vector<Entity> Scene::loadGlTF(const std::string& path)
    {
        tinygltf::Model model;
        if (!GlTFLoader::loadModel(path, model))
        {
            return {};
        }

        std::vector<Entity> entitiesCreated;
        entitiesCreated.reserve(GlTFLoader::getPrimitiveCount(model));

        for (auto& node : model.nodes)
        {
            auto& mesh = model.meshes[node.mesh];
            
            for (tinygltf::Primitive& primitive : mesh.primitives)
            {
                std::unique_ptr<VertexArray> vertexArray;
                bool vertexSuccess = GlTFLoader::getVertexArray(model, primitive, vertexArray);

                std::unique_ptr<Texture> texture;
                bool textureSuccess = GlTFLoader::getTexture(model, primitive, texture);

                if (!vertexSuccess || !textureSuccess)
                {
                    continue;
                }

                entitiesCreated.push_back(createEntity());
                entitiesCreated.back().addComponent<TransformComponent>();
                auto& meshComp = entitiesCreated.back().addComponent<MeshComponent>();
                meshComp.m_vertexArray = std::move(vertexArray);
                meshComp.m_texture = std::move(texture);
            }
        }
        return entitiesCreated;
    }
} // namespace Turbo
