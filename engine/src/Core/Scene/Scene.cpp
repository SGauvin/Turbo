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
        m_shader.setFloat3("material.ambient", glm::vec3(0.8f, 0.8f, 0.8f));
        m_shader.setFloat3("material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        m_shader.setFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        m_shader.setFloat("material.shininess", 256.0f);

        m_shader.setFloat3("light.position", m_lightPos);
        m_shader.setFloat3("light.ambient",  glm::vec3(0.4f, 0.4f, 0.4f));
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


    void Scene::loadGlTF(const std::string& path)
    {
        Turbo::BufferLayout layout = {
            { Turbo::DataType::Float3, "a_position" },
            { Turbo::DataType::Float3, "a_color" },
            { Turbo::DataType::Float2, "a_textCoord" },
        };

        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string errors;
        std::string warnings;

        bool hasLoaded = loader.LoadASCIIFromFile(&model, &errors, &warnings, path);

        if (errors.empty() == false)
        {
            TURBO_ENGINE_WARNING(errors);
        }

        if (warnings.empty() == false)
        {
            TURBO_ENGINE_ERROR(warnings);
        }

        if (hasLoaded == false)
        {
            TURBO_ENGINE_ERROR("Failed to load {}", path);
            return;
        }

        if (model.meshes.empty())
        {
            TURBO_ENGINE_ERROR("No meshes in file {}", path);
            return;
        }

        for (std::size_t i = 0; i < model.meshes.size(); i++)
        {
            tinygltf::Mesh& mesh = model.meshes[i];
            std::unique_ptr<VertexArray> vertexArray;
            std::unique_ptr<Texture> texture;
            
            std::vector<float> data;

            if (mesh.primitives.size() > 1)
            {
                TURBO_ENGINE_ERROR("NOT SUPPORTING MORE THAN 1 PRIMITIVE ATM");
                continue;
            }
            for (tinygltf::Primitive& primitive : mesh.primitives)
            {
                if (primitive.mode != 4)
                {
                    TURBO_ENGINE_ERROR("Not mode 4");
                    continue;
                }

                const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes["POSITION"]];
                const tinygltf::BufferView& positionBufferView = model.bufferViews[positionAccessor.bufferView];
                const tinygltf::Buffer& positionBuffer = model.buffers[positionBufferView.buffer];
                const float* positions = reinterpret_cast<const float*>(&positionBuffer.data[positionBufferView.byteOffset + positionAccessor.byteOffset]);

                const tinygltf::Accessor& normalAccessor = model.accessors[primitive.attributes["NORMAL"]];
                const tinygltf::BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
                const tinygltf::Buffer& normalBuffer = model.buffers[normalBufferView.buffer];
                const float* normals = reinterpret_cast<const float*>(&normalBuffer.data[normalBufferView.byteOffset + normalAccessor.byteOffset]);

                const tinygltf::Accessor& texCoord0Accessor = model.accessors[primitive.attributes["TEXCOORD_0"]];
                const tinygltf::BufferView& texCoord0BufferView = model.bufferViews[texCoord0Accessor.bufferView];
                const tinygltf::Buffer& texCoord0Buffer = model.buffers[texCoord0BufferView.buffer];
                const float* texCoord0 = reinterpret_cast<const float*>(&texCoord0Buffer.data[texCoord0BufferView.byteOffset + texCoord0Accessor.byteOffset]);

                if (positionAccessor.count != normalAccessor.count || texCoord0Accessor.count != positionAccessor.count)
                {
                    TURBO_ENGINE_ERROR("Not the same count???");
                    continue;
                }
                
                data.reserve(data.size() + positionAccessor.count * 8);

                for (std::size_t i = 0; i < positionAccessor.count; i++)
                {
                    data.push_back(positions[i * 3]);
                    data.push_back(positions[i * 3 + 1]);
                    data.push_back(positions[i * 3 + 2]);
                    data.push_back(normals[i * 3]);
                    data.push_back(normals[i * 3 + 1]);
                    data.push_back(normals[i * 3 + 2]);
                    data.push_back(texCoord0[i * 2]);
                    data.push_back(texCoord0[i * 2 + 1]);
                }

                const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
                const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];
                const std::uint16_t* indices = reinterpret_cast<const std::uint16_t*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);

                std::vector<std::uint32_t> indices32Bits;
                if (indicesAccessor.count == 0)
                {
                    indices32Bits.reserve(positionAccessor.count);
                    for (std::size_t i = 0; i < positionAccessor.count; i++)
                    {
                        indices32Bits.push_back(i);
                    }
                }
                else
                {
                    indices32Bits.reserve(indicesAccessor.count);
                    for (std::size_t i = 0; i < indicesAccessor.count; i++)
                    {
                        indices32Bits.push_back(indices[i]);
                    }
                }

                std::shared_ptr<Turbo::VertexBuffer> vertexBuffer = std::make_shared<Turbo::VertexBuffer>(std::span<float>(data.data(), data.size()));
                vertexBuffer->setLayout(layout);

                std::shared_ptr<Turbo::IndexBuffer> indexBuffer = std::make_shared<Turbo::IndexBuffer>(std::span<const std::uint32_t>(indices32Bits.data(), indices32Bits.size()));

                vertexArray = std::make_unique<Turbo::VertexArray>();
                vertexArray->setVertexBuffer(vertexBuffer);
                vertexArray->setIndexBuffer(indexBuffer);

                int emissiveTextureIndex = model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index;
                if (emissiveTextureIndex >= 0)
                {
                    tinygltf::Texture tinytexture = model.textures[emissiveTextureIndex];
                    if (tinytexture.source < 0)
                    {
                        return;
                    }

                    texture = std::make_unique<Texture>();
                    texture->load(model.images[tinytexture.source]);
                }
            }

            auto entity = createEntity();
            entity.addComponent<TransformComponent>();
            MeshComponent& meshComp = entity.addComponent<MeshComponent>();
            meshComp.m_vertexArray = std::move(vertexArray);
            meshComp.m_texture = std::move(texture);
        }
    }
} // namespace Turbo