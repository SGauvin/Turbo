#ifndef INCLUDED_TURBO_BUFFERLAYOUT_H
#define INCLUDED_TURBO_BUFFERLAYOUT_H

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

        DataType getDataType() const;
        std::uint8_t getSize() const;
        std::uint32_t& getOffset();
        std::uint8_t getComponentCount() const;
        bool isNormalized() const;

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
        BufferLayout(std::initializer_list<BufferElement> elements);
        BufferElement& operator[](std::size_t i);
        std::size_t size() const;
        std::uint32_t getStride() const;

    private:
        std::vector<BufferElement> m_elements;
        std::uint32_t m_stride;
    };
} // namespace Turbo

#endif