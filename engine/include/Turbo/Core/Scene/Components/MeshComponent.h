#pragma once

#include <glm/glm.hpp>
#include <tiny_gltf.h>
#include <algorithm>

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Settings.h"
#include "Turbo/Core/Renderer/Abstraction/VertexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/IndexBuffer.h"
#include "Turbo/Core/Renderer/Abstraction/VertexArray.h"

namespace Turbo
{
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
    };

    class TestMeshComponent
    {
    public:
        TestMeshComponent(const std::string& file)
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

            bool hasLoaded = loader.LoadASCIIFromFile(&model, &errors, &warnings, file);

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
                TURBO_ENGINE_ERROR("Failed to load {}", file);
                return;
            }

            if (model.meshes.empty())
            {
                TURBO_ENGINE_ERROR("No meshes in file {}", file);
                return;
            }

            tinygltf::Mesh& mesh = model.meshes[0];

            std::vector<float> data;
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

                m_vertexArray = std::make_unique<Turbo::VertexArray>();
                m_vertexArray->setVertexBuffer(vertexBuffer);
                m_vertexArray->setIndexBuffer(indexBuffer);
            }
        }

        std::unique_ptr<VertexArray> m_vertexArray;
    };

    class MeshComponent
    {
    public:
        MeshComponent()
        {
            Turbo::BufferLayout layout = {
                { Turbo::DataType::Float3, "a_position" },
                { Turbo::DataType::Float3, "a_color" },
                { Turbo::DataType::Float2, "a_textCoord" },
            };

            float vertices[] = {
                // positions          // normals           // texture coords
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
            };

            std::uint32_t indices[] = {
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 ,30, 31, 32, 33, 34, 35
            };

            std::shared_ptr<Turbo::VertexBuffer> vertexBuffer =
                std::make_shared<Turbo::VertexBuffer>(std::span<float>(vertices, sizeof(vertices) / sizeof(float)));
            vertexBuffer->setLayout(layout);

            std::shared_ptr<Turbo::IndexBuffer> indexBuffer =
                std::make_shared<Turbo::IndexBuffer>(std::span<std::uint32_t>(indices, sizeof(indices) / sizeof(std::uint32_t)));

            m_vertexArray = std::make_unique<Turbo::VertexArray>();
            m_vertexArray->setVertexBuffer(vertexBuffer);
            m_vertexArray->setIndexBuffer(indexBuffer);
        }

        VertexArray const * getVertexArray() const
        {
            return m_vertexArray.get();
        }

    private:
        std::vector<Vertex> m_vertices;
        std::vector<std::uint32_t> m_indices;

        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;
        std::unique_ptr<VertexArray> m_vertexArray;
    };
} // namespace Turbo
