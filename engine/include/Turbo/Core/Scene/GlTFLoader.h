#pragma once

#include <span>
#include <string>
#include <tiny_gltf.h>

#include "Turbo/Core/Log.h"

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

            std::vector<std::uint32_t> indices32Bits;
            if (indicesAccessor.count == 0)
            {
                const tinygltf::Accessor& positionAccessor = model.accessors[primitive.attributes["POSITION"]];
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

    } // namespace GlTFLoader
} // namespace Turbo