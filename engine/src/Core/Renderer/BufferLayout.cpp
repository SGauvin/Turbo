#include "Turbo/Core/Renderer/BufferLayout.h"
#include <array>
#include "Turbo/Core/Log.h"

namespace
{
    std::uint8_t getDataTypeSize(Turbo::DataType type)
    {
        static constexpr std::array dataTypeSizes = std::to_array<std::uint8_t>({
            4 * 1, // Float
            4 * 2, // Float2
            4 * 3, // Float3
            4 * 4, // Float4
            4 * 1, // Int
            4 * 2, // Int2
            4 * 3, // Int3
            4 * 3, // Int4
            1, // Bool
        });
        using EnumType = std::underlying_type_t<decltype(type)>;
        TURBO_ASSERT(static_cast<EnumType>(type) < dataTypeSizes.size(), "Invalid DataType");

        // NOLINTNEXTLINE
        return dataTypeSizes[static_cast<std::uint8_t>(type)];
    }

    std::uint8_t getComponentCount(Turbo::DataType type)
    {
        static constexpr std::array componentCount = std::to_array<std::uint8_t>({
            1, // Float
            2, // Float2
            3, // Float3
            4, // Float4
            1, // Int
            2, // Int2
            3, // Int3
            4, // Int4
            1, // Bool
        });
        using EnumType = std::underlying_type_t<decltype(type)>;
        TURBO_ASSERT(static_cast<EnumType>(type) < componentCount.size(), "Invalid DataType");

        // NOLINTNEXTLINE
        return componentCount[static_cast<std::uint8_t>(type)];
    }

} // namespace
namespace Turbo
{
    BufferElement::BufferElement(DataType type, const std::string& name, bool isNormalized)
        : m_type(type)
        , m_name(std::move(name))
        , m_size(getDataTypeSize(type))
        , m_offset(0)
        , m_componentCount(::getComponentCount(type))
        , m_isNormalized(isNormalized)
    {
    }

    DataType BufferElement::getDataType() const
    {
        return m_type;
    }

    std::uint8_t BufferElement::getSize() const
    {
        return m_size;
    }

    const std::uint32_t& BufferElement::getOffset() const
    {
        return m_offset;
    }

    std::uint8_t BufferElement::getComponentCount() const
    {
        return m_componentCount;
    }

    bool BufferElement::isNormalized() const
    {
        return m_isNormalized;
    }

    BufferLayout::BufferLayout()
        : m_elements{}
        , m_stride(0)
    {
    }

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

    const BufferElement& BufferLayout::operator[](std::size_t i) const
    {
        return m_elements[i];
    }

    std::size_t BufferLayout::size() const
    {
        return m_elements.size();
    }

    std::uint32_t BufferLayout::getStride() const
    {
        return m_stride;
    }
} // namespace Turbo
