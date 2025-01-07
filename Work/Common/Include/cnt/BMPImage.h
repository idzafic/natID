// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <td/String.h>

namespace cnt
{

class BMPImage
{
    # pragma pack (push, 1)
    struct BMPHeader
    {
        std::uint16_t bfType;
        std::uint32_t bfSize;
        std::uint16_t bfReserved1;
        std::uint16_t bfReserved2;
        std::uint32_t bfOffBits;
        std::uint32_t biSize;
        std::int32_t  biWidth;
        std::int32_t  biHeight;
        std::uint16_t biPlanes;
        std::uint16_t biBitCount;
        std::uint32_t biCompression;
        std::uint32_t biSizeImage;
        std::int32_t  biXPelsPerMeter;
        std::int32_t  biYPelsPerMeter;
        std::uint32_t biClrUsed;
        std::uint32_t biClrImportant;
    };

    static_assert(sizeof(BMPHeader) == 54);
    # pragma pack (pop)

public:
    
    struct RGB
    {
        double r = 0.0;
        double g = 0.0;
        double b = 0.0;
        constexpr RGB() = default;
        explicit constexpr RGB(double _rgb) noexcept
            : r(_rgb), g(_rgb), b(_rgb) {}
        constexpr RGB(double _r, double _g, double _b) noexcept
            : r(_r), g(_g), b(_b) {}
    };
    
private:

    std::vector<RGB> _data;
    
    td::UINT4 _width = 0;
    td::UINT4 _height = 0;

    bool inBounds(std::int32_t y, std::int32_t x) const noexcept
    {
        return (0 <= y) && (y < _height) && (0 <= x) && (x < _width);
    }

    static constexpr std::uint8_t ToUint8(double x) noexcept
    {
        return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
    }

public:

    BMPImage() = default;

    BMPImage(std::size_t width, std::size_t height)
        : _data(width * height)
        , _width(static_cast<td::UINT4>(width))
        , _height(static_cast<td::UINT4>(height)) {}

    void set(std::int32_t x, std::int32_t y, const RGB& color)
    {
        if (!inBounds(y, x))
        {
            return;
        }

        _data[static_cast<std::size_t>(y) * _width + x] = color;
    }

    td::UINT4 width() const
    {
        return _width;
    }

    td::UINT4 height() const
    {
        return _height;
    }

    bool save(const td::String& path)
    {
        const td::UINT4  rowSize = _width * 3 + _width % 4;
        const td::UINT4 bmpsize = rowSize * _height;
        const BMPHeader header =
        {
            0x4d42,
            static_cast<std::uint32_t>(bmpsize + sizeof(BMPHeader)),
            0,
            0,
            sizeof(BMPHeader),
            40,
            (std::int32_t)_width,
            (std::int32_t)_height,
            1,
            24,
            0,
            bmpsize,
            0,
            0,
            0,
            0
        };

        if (std::ofstream ofs{ path.c_str(), std::ios_base::binary })
        {
            ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));

            std::vector<std::uint8_t> line(rowSize);

            for (std::int32_t y = _height - 1; -1 < y; --y)
            {
                size_t pos = 0;

                for (std::int32_t x = 0; x < _width; ++x)
                {
                    const RGB& col = _data[static_cast<std::size_t>(y) * _width + x];
                    line[pos++] = ToUint8(col.b);
                    line[pos++] = ToUint8(col.g);
                    line[pos++] = ToUint8(col.r);
                }

                ofs.write(reinterpret_cast<const char*>(line.data()), line.size());
            }

            return true;
        }
        else
        {
            return false;
        }
    }
};

} //namespace cnt



