#ifndef INCLUDED_TURBO_BUFFERLAYOUT_H
#define INCLUDED_TURBO_BUFFERLAYOUT_H

#include <string>
#include <initializer_list>

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
        UInt,
        UInt2,
        UInt3,
        UInt4,
        Bool,
    };

    class BufferElement
    {
    public:
        BufferElement(DataType type, const std::string& name);
    private:
        DataType m_type;
        std::string m_name;
        std::uint8_t m_size;
        std::uint16_t m_offset;
    };

    class BufferLayout
    {
    public:
        BufferLayout(std::initializer_list<BufferElement>);
    private:
};
}

#endif