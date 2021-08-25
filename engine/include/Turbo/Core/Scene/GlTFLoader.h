#pragma once

#include <span>
#include <string>
#include <tiny_gltf.h>

#include "Turbo/Core/Log.h"
#include "Turbo/Core/Renderer/Abstraction/Texture.h"

namespace Turbo
{
    namespace GlTFLoader
    {
        bool loadModel(const std::string& path, tinygltf::Model& outModel)
        {
            std::size_t extensionStart = path.find_last_of('.');
            if (extensionStart == std::string::npos)
            {
                TURBO_ENGINE_ERROR("{} has no extension", path);
                return false;
            }

            std::string_view strView = path;
            std::string_view extension = strView.substr(extensionStart, path.size() - extensionStart);

            tinygltf::TinyGLTF loader;
            std::string errors;
            std::string warnings;

            bool hasLoaded; 
            if (extension == ".gltf")
            {
                hasLoaded = loader.LoadASCIIFromFile(&outModel, &errors, &warnings, path);
            }
            else if (extension == ".glb")
            {
                hasLoaded = loader.LoadBinaryFromFile(&outModel, &errors, &warnings, path);
            }
            else
            {
                TURBO_ENGINE_ERROR("{} has no valid extension (extension is {})", path, extension);
                return false;
            }

            if (warnings.empty() == false)
            {
                TURBO_ENGINE_WARNING(warnings);
            }
            if (errors.empty() == false)
            {
                TURBO_ENGINE_ERROR(errors);
                return false;
            }

            if (hasLoaded == false)
            {
                TURBO_ENGINE_ERROR("Failed to load {}", path);
                return false;
            }
            return true;
        }

        std::size_t getPrimitiveCount(const tinygltf::Model& model)
        {
            std::size_t primitiveCount = 0;
            for (std::size_t i = 0; i < model.meshes.size(); i++)
            {
                primitiveCount += model.meshes[i].primitives.size();
            }
            return primitiveCount;
        }

        template<typename T>
        bool getPrimitiveAttribute(tinygltf::Model& model, tinygltf::Primitive& primitive, const std::string& attributeName, std::span<T>& outData)
        {
            const tinygltf::Accessor& accessor = model.accessors[primitive.attributes[attributeName]];
            const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
            T* positions = reinterpret_cast<T*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

            outData = std::span<T>(positions, accessor.count);
            return true;
        }

        bool getVertexPositions(tinygltf::Model& model, tinygltf::Primitive& primitive, std::span<const float>& outData)
        {
            return getPrimitiveAttribute(model, primitive, "POSITION", outData);
        }

        bool getVertexNormals(tinygltf::Model& model, tinygltf::Primitive& primitive, std::span<const float>& outData)
        {
            return getPrimitiveAttribute(model, primitive, "NORMAL", outData);
        }

        bool getVertexTexCoords(tinygltf::Model& model, tinygltf::Primitive& primitive, std::span<const float>& outData)
        {
            return getPrimitiveAttribute(model, primitive, "TEXCOORD_0", outData);
        }

        bool getVertexIndices(tinygltf::Model& model, tinygltf::Primitive& primitive, std::vector<std::uint32_t>& outData)
        {
            const tinygltf::Accessor& indicesAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indicesBufferView = model.bufferViews[indicesAccessor.bufferView];
            const tinygltf::Buffer& indicesBuffer = model.buffers[indicesBufferView.buffer];
            const std::uint16_t* indices = reinterpret_cast<const std::uint16_t*>(&indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]);

            if (indicesAccessor.count == 0)
            {
                TURBO_WARNING("No indices found!");
                const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes["POSITION"]];
                outData.reserve(positionAccessor.count);
                for (std::size_t i = 0; i < positionAccessor.count; i++)
                {
                    outData.push_back(i);
                }
            }
            else
            {
                outData.reserve(indicesAccessor.count);
                for (std::size_t i = 0; i < indicesAccessor.count; i++)
                {
                    outData.push_back(indices[i]);
                }
            }

            return true;
        }

        bool createVertexData(const std::span<const float>& positions, const std::span<const float>& normals, const std::span<const float>& texCoords, std::vector<float>& outVertexData)
        {
            outVertexData.clear();
            outVertexData.reserve(positions.size() * 8);
            for (std::size_t i = 0; i < positions.size(); i++)
            {
                outVertexData.push_back(positions[i * 3]);
                outVertexData.push_back(positions[i * 3 + 1]);
                outVertexData.push_back(positions[i * 3 + 2]);
                outVertexData.push_back(normals[i * 3]);
                outVertexData.push_back(normals[i * 3 + 1]);
                outVertexData.push_back(normals[i * 3 + 2]);
                outVertexData.push_back(texCoords[i * 2]);
                outVertexData.push_back(texCoords[i * 2 + 1]);
            }
            return true;
        }

        bool getVertexArray(tinygltf::Model& model, tinygltf::Primitive& primitive, std::unique_ptr<VertexArray>& outVertexArray)
        {
            if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
            {
                TURBO_ENGINE_ERROR("Must be triangles");
                return false;
            }

            std::span<const float> positions, normals, texCoords;
            GlTFLoader::getVertexPositions(model, primitive, positions);
            GlTFLoader::getVertexNormals(model, primitive, normals);
            GlTFLoader::getVertexTexCoords(model, primitive, texCoords);

            if (positions.size() != normals.size() || positions.size() != texCoords.size())
            {
                TURBO_ENGINE_ERROR("Not the same count???");
                return false;
            }
            
            std::vector<float> data;
            GlTFLoader::createVertexData(positions, normals, texCoords, data);

            std::vector<std::uint32_t> indices;
            GlTFLoader::getVertexIndices(model, primitive, indices);

            std::shared_ptr<Turbo::VertexBuffer> vertexBuffer = std::make_shared<Turbo::VertexBuffer>(std::span<float>(data));
            
            Turbo::BufferLayout layout = {
                { Turbo::DataType::Float3, "a_position" },
                { Turbo::DataType::Float3, "a_color" },
                { Turbo::DataType::Float2, "a_textCoord" },
            };
            vertexBuffer->setLayout(layout);

            std::shared_ptr<Turbo::IndexBuffer> indexBuffer = std::make_shared<Turbo::IndexBuffer>(std::span<std::uint32_t>(indices));

            outVertexArray = std::make_unique<Turbo::VertexArray>();
            outVertexArray->setVertexBuffer(vertexBuffer);
            outVertexArray->setIndexBuffer(indexBuffer);

            return true;
        }

        bool getTexture(tinygltf::Model& model, tinygltf::Primitive& primitive, std::unique_ptr<Texture>& outTexture)
        {
            int emissiveTextureIndex = model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index;
            if (emissiveTextureIndex < 0)
            {
                return false;
            }

            tinygltf::Texture tinytexture = model.textures[emissiveTextureIndex];
            if (tinytexture.source < 0)
            {
                return false;
            }

            outTexture = std::make_unique<Texture>();
            outTexture->load(model.images[tinytexture.source]);

            return true;
        }

    } // namespace GlTFLoader
} // namespace Turbo