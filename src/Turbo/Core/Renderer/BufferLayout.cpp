#include "Turbo/Core/Renderer/BufferLayout.h"

namespace
{
    std::uint8_t getDataTypeSize(Turbo::DataType type)
    {
        static constexpr std::uint8_t dataTypeSizes[] = {
            4 * 1, // Float
            4 * 2, // Float2
            4 * 3, // Float3
            4 * 4, // Float4
            4 * 1, // Int
            4 * 2, // Int2
            4 * 3, // Int3
            4 * 3, // Int4
            4 * 1, // UInt
            4 * 2, // UInt2
            4 * 3, // UInt3
            4 * 3, // UInt4
            1, // Bool
        };
        return dataTypeSizes[static_cast<std::uint8_t>(type)];
    }
} // namespace
namespace Turbo
{
    BufferElement::BufferElement(DataType type, const std::string& name)
        : m_type(type)
        , m_name(name)
        , m_size(getDataTypeSize(type))
    {}

    BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
        : m_elements(elements)
        , m_stride(0)
    {
        std::uint32_t currentOffset = 0;
        for (auto& element : m_elements)
        {
            element.m_offset = currentOffset;
            currentOffset += element.m_size;
        }
        m_stride = currentOffset;
    }
} // namespace Turbo