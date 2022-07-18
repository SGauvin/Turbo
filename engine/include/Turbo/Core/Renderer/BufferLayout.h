#pragma once

#include <initializer_list>
#include <string>
#include <vector>

namespace Turbo
{
    enum class DataType : uint8_t
    {
        Float = 0,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool,
    };

    class BufferElement
    {
    public:
        friend class BufferLayout;
        BufferElement(DataType type, const std::string& name, bool isNormalized = false);

        [[nodiscard]] DataType getDataType() const;
        [[nodiscard]] std::uint8_t getSize() const;
        [[nodiscard]] const std::uint32_t& getOffset() const;
        [[nodiscard]] std::uint8_t getComponentCount() const;
        [[nodiscard]] bool isNormalized() const;

    private:
        DataType m_type;
        std::string m_name;
        std::uint8_t m_size;
        std::uint32_t m_offset;
        std::uint8_t m_componentCount;
        bool m_isNormalized;
    };

    class BufferLayout
    {
    public:
        BufferLayout();
        BufferLayout(std::initializer_list<BufferElement> elements);
        const BufferElement& operator[](std::size_t i) const;
        [[nodiscard]] std::size_t size() const;
        [[nodiscard]] std::uint32_t getStride() const;

    private:
        std::vector<BufferElement> m_elements;
        std::uint32_t m_stride;
    };
} // namespace Turbo
