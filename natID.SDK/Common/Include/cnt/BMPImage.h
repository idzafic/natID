// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BMPImage.h
    @brief In-memory RGBA image with save support for BMP and PNG formats. */
#pragma once
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <td/String.h>
#include <cnt/SafeFullVector.h>
#include <td/Color.h>
#include <zlib/zlib.h>

namespace cnt
{

/// @brief Stores an RGBA pixel image and provides methods to save it as BMP or PNG files.
class BMPImage
{
    # pragma pack (push, 1)
    /// @brief Packed BMP file header containing both BITMAPFILEHEADER and BITMAPINFOHEADER fields.
    struct BMPHeader
    {
        std::uint16_t bfType;       ///< BMP signature ("BM").
        std::uint32_t bfSize;       ///< Total file size in bytes.
        std::uint16_t bfReserved1;  ///< Reserved; must be zero.
        std::uint16_t bfReserved2;  ///< Reserved; must be zero.
        std::uint32_t bfOffBits;    ///< Offset from start of file to pixel data.
        std::uint32_t biSize;       ///< Size of the info header in bytes.
        std::int32_t  biWidth;      ///< Image width in pixels.
        std::int32_t  biHeight;     ///< Image height in pixels (positive = bottom-up).
        std::uint16_t biPlanes;     ///< Number of color planes (must be 1).
        std::uint16_t biBitCount;   ///< Bits per pixel.
        std::uint32_t biCompression;///< Compression type (0 = uncompressed).
        std::uint32_t biSizeImage;  ///< Size of pixel data in bytes.
        std::int32_t  biXPelsPerMeter; ///< Horizontal resolution (pixels per meter).
        std::int32_t  biYPelsPerMeter; ///< Vertical resolution (pixels per meter).
        std::uint32_t biClrUsed;    ///< Number of colors in the color table.
        std::uint32_t biClrImportant; ///< Number of important colors used.
    };

    static_assert(sizeof(BMPHeader) == 54);
    # pragma pack (pop)

public:
    /// @brief Output image format selector.
    enum class Type : td::BYTE {BMPrgb, BMPrgba, PNGr, PNGg, PNGb, PNGa, PNGrgb, PNGrgba};

    /// @brief Represents a single pixel with red, green, blue, and alpha components.
    struct RGBA
    {
        td::BYTE r = 0;   ///< Red channel (0-255).
        td::BYTE g = 0;   ///< Green channel (0-255).
        td::BYTE b = 0;   ///< Blue channel (0-255).
        td::BYTE a = 255; ///< Alpha channel (0-255, 255 = fully opaque).

        /// @brief Default constructor; initialises to transparent black.
        RGBA() = default;

        /// @brief Constructs a grayscale pixel from a float intensity and optional alpha.
        /// @param rgb Gray intensity in [0.0, 1.0].
        /// @param alpha Alpha value in [0.0, 1.0], defaults to 1.
        explicit RGBA(float rgb, float alpha = 1) noexcept
        : r(std::floor(rgb*255+0.5f))
        , g(std::floor(rgb*255+0.5f))
        , b(std::floor(rgb*255+0.5f))
        , a(std::floor(alpha*255+0.5f))
        {
        }

        /// @brief Constructs a grayscale pixel from a byte intensity and optional alpha.
        /// @param rgb Gray intensity (0-255).
        /// @param alpha Alpha value (0-255), defaults to 255.
        explicit RGBA(td::BYTE rgb, td::BYTE alpha=255) noexcept
        : r(rgb)
        , g(rgb)
        , b(rgb)
        , a(alpha)
        {
        }

        /// @brief Constructs a colour pixel from individual float channels.
        /// @param rIn Red channel in [0.0, 1.0].
        /// @param gIn Green channel in [0.0, 1.0].
        /// @param bIn Blue channel in [0.0, 1.0].
        /// @param alpha Alpha channel in [0.0, 1.0], defaults to 1.
        RGBA(float rIn, float gIn, float bIn, float alpha = 1) noexcept
        : r(std::floor(rIn*255+0.5f))
        , g(std::floor(gIn*255+0.5f))
        , b(std::floor(bIn*255+0.5f))
        , a(std::floor(alpha*255+0.5f))
        {
        }

        /// @brief Constructs an RGBA pixel from a td::Color value.
        /// @param color Source color object providing red(), green(), blue(), alpha() accessors.
        RGBA(const td::Color& color) noexcept
        : r(color.red())
        , g(color.green())
        , b(color.blue())
        , a(color.alpha())
        {
        }
    };

private:

    cnt::SafeFullVector<RGBA> _data; ///< Flat pixel buffer stored in row-major order.

    td::UINT4 _width = 0;  ///< Image width in pixels.
    td::UINT4 _height = 0; ///< Image height in pixels.

    /// @brief Returns true if (y, x) is inside the image bounds.
    /// @param y Row index.
    /// @param x Column index.
    /// @return True when the coordinate is within the valid pixel range.
    bool inBounds(std::int32_t y, std::int32_t x) const noexcept
    {
        return (0 <= y) && (y < _height) && (0 <= x) && (x < _width);
    }

//    static constexpr std::uint8_t ToUint8(double x) noexcept
//    {
//        return x >= 1.0 ? 255 : x <= 0.0 ? 0 : static_cast<std::uint8_t>(x * 255.0 + 0.5);
//    }

    /// @brief Saves the image as a 24-bit RGB BMP file.
    /// @param path Destination file path.
    /// @return True on success, false if the file could not be opened.
    bool saveBMPrgb(const td::String& path)
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
                    const RGBA& col = _data[static_cast<std::size_t>(y) * _width + x];
                    line[pos++] = col.b;
                    line[pos++] = col.g;
                    line[pos++] = col.r;
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


    /// @brief Saves the image as a 32-bit RGBA BMP file.
    /// @param path Destination file path.
    /// @return True on success, false if the file could not be opened.
    bool saveBMPrgba(const td::String& path)
    {
        const td::UINT4 rowSize = _width * 4; // 4 bytes per pixel (RGBA)
        const td::UINT4 bmpsize = rowSize * _height;
        const BMPHeader header = {
            0x4d42,
            static_cast<std::uint32_t>(bmpsize + sizeof(BMPHeader)),
            0,
            0,
            sizeof(BMPHeader),
            40,
            static_cast<std::int32_t>(_width),
            static_cast<std::int32_t>(_height),
            1,
            32, // 32 bits per pixel for RGBA
            0,
            bmpsize,
            0,
            0,
            0,
            0
        };

        if (std::ofstream ofs{path.c_str(), std::ios_base::binary}) {
            ofs.write(reinterpret_cast<const char*>(&header), sizeof(header));

            std::vector<std::uint8_t> line(rowSize);
            for (std::int32_t y = _height - 1; -1 < y; --y) {
                size_t pos = 0;
                for (std::int32_t x = 0; x < _width; ++x) {
                    const RGBA& col = _data[static_cast<std::size_t>(y) * _width + x];
                    line[pos++] = col.b;
                    line[pos++] = col.g;
                    line[pos++] = col.r;
                    line[pos++] = col.a;
                }
                ofs.write(reinterpret_cast<const char*>(line.data()), line.size());
            }
            return true;
        } else {
            return false;
        }
    }

    /// @brief Saves the red channel of the image as a grayscale PNG file.
    /// @param path Destination file path.
    /// @return True on success, false on compression or I/O error.
    bool savePNGr(const td::String& path)
    {
        // PNG requires a filter byte at the start of each scanline
        const int bytesPerPixel = 1; // Grayscale - single channel

        // Prepare data with filter bytes (1 byte per scanline)
        std::vector<unsigned char> rawData(_height * (_width * bytesPerPixel + 1));

        for (size_t y = 0; y < _height; ++y) {
            // Add filter byte (0 = no filter) at the start of each scanline
            rawData[y * (_width * bytesPerPixel + 1)] = 0;

            for (size_t x = 0; x < _width; ++x) {
                const RGBA& color = _data[y * _width + x];
                // Only save the red channel as grayscale
                size_t pos = y * (_width * bytesPerPixel + 1) + 1 + x * bytesPerPixel;
                rawData[pos] = color.r;
            }
        }

        // zlib compression
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return false;
        }

        zs.next_in = rawData.data();
        zs.avail_in = (unsigned int)rawData.size();

        int ret;
        unsigned char outBuffer[4096]; // Use a buffer
        std::vector<unsigned char> compressedData;

        do {
            zs.next_out = outBuffer;
            zs.avail_out = sizeof(outBuffer);
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&zs);
                return false;
            }

            size_t have = sizeof(outBuffer) - zs.avail_out;
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + have);

        } while (zs.avail_out == 0);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return false;
        }

        // PNG Header and Data chunks.
        std::vector<unsigned char> pngData = {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a // PNG signature
        };

        // IHDR chunk
        {
            std::vector<unsigned char> ihdrData;
            ihdrData.reserve(13);
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_width >> (24 - i * 8)) & 0xFF);  // Width
            }
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_height >> (24 - i * 8)) & 0xFF); // Height
            }
            ihdrData.push_back(8);             // Bit depth
            ihdrData.push_back(0);             // Color type (0 = Grayscale)
            ihdrData.push_back(0);             // Compression method
            ihdrData.push_back(0);             // Filter method
            ihdrData.push_back(0);             // Interlace method

            // IHDR chunk length and type
            std::vector<unsigned char> ihdrChunk = createChunk("IHDR", ihdrData);
            pngData.insert(pngData.end(), ihdrChunk.begin(), ihdrChunk.end());
        }

        // IDAT chunk
        {
            std::vector<unsigned char> idatChunk = createChunk("IDAT", compressedData);
            pngData.insert(pngData.end(), idatChunk.begin(), idatChunk.end());
        }

        // IEND chunk
        {
           std::vector<unsigned char> iendChunk = createChunk("IEND", {});
           pngData.insert(pngData.end(), iendChunk.begin(), iendChunk.end());
        }

        // Write to file
        std::ofstream file(path.c_str(), std::ios::binary);
        if (!file) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(pngData.data()), pngData.size());
        return true;
    }

    /// @brief Saves the green channel of the image as a grayscale PNG file.
    /// @param path Destination file path.
    /// @return True on success, false on compression or I/O error.
    bool savePNGg(const td::String& path)
    {
        // PNG requires a filter byte at the start of each scanline
        const int bytesPerPixel = 1; // Grayscale - single channel

        // Prepare data with filter bytes (1 byte per scanline)
        std::vector<unsigned char> rawData(_height * (_width * bytesPerPixel + 1));

        for (size_t y = 0; y < _height; ++y) {
            // Add filter byte (0 = no filter) at the start of each scanline
            rawData[y * (_width * bytesPerPixel + 1)] = 0;

            for (size_t x = 0; x < _width; ++x) {
                const RGBA& color = _data[y * _width + x];
                // Only save the green channel as grayscale
                size_t pos = y * (_width * bytesPerPixel + 1) + 1 + x * bytesPerPixel;
                rawData[pos] = color.g;
            }
        }

        // zlib compression
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return false;
        }

        zs.next_in = rawData.data();
        zs.avail_in = (unsigned int)rawData.size();

        int ret;
        unsigned char outBuffer[4096]; // Use a buffer
        std::vector<unsigned char> compressedData;

        do {
            zs.next_out = outBuffer;
            zs.avail_out = sizeof(outBuffer);
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&zs);
                return false;
            }

            size_t have = sizeof(outBuffer) - zs.avail_out;
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + have);

        } while (zs.avail_out == 0);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return false;
        }

        // PNG Header and Data chunks.
        std::vector<unsigned char> pngData = {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a // PNG signature
        };

        // IHDR chunk
        {
            std::vector<unsigned char> ihdrData;
            ihdrData.reserve(13);
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_width >> (24 - i * 8)) & 0xFF);  // Width
            }
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_height >> (24 - i * 8)) & 0xFF); // Height
            }
            ihdrData.push_back(8);             // Bit depth
            ihdrData.push_back(0);             // Color type (0 = Grayscale)
            ihdrData.push_back(0);             // Compression method
            ihdrData.push_back(0);             // Filter method
            ihdrData.push_back(0);             // Interlace method

            // IHDR chunk length and type
            std::vector<unsigned char> ihdrChunk = createChunk("IHDR", ihdrData);
            pngData.insert(pngData.end(), ihdrChunk.begin(), ihdrChunk.end());
        }

        // IDAT chunk
        {
            std::vector<unsigned char> idatChunk = createChunk("IDAT", compressedData);
            pngData.insert(pngData.end(), idatChunk.begin(), idatChunk.end());
        }

        // IEND chunk
        {
           std::vector<unsigned char> iendChunk = createChunk("IEND", {});
           pngData.insert(pngData.end(), iendChunk.begin(), iendChunk.end());
        }

        // Write to file
        std::ofstream file(path.c_str(), std::ios::binary);
        if (!file) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(pngData.data()), pngData.size());
        return true;
    }

    /// @brief Saves the blue channel of the image as a grayscale PNG file.
    /// @param path Destination file path.
    /// @return True on success, false on compression or I/O error.
    bool savePNGb(const td::String& path)
    {
        // PNG requires a filter byte at the start of each scanline
        const int bytesPerPixel = 1; // Grayscale - single channel

        // Prepare data with filter bytes (1 byte per scanline)
        std::vector<unsigned char> rawData(_height * (_width * bytesPerPixel + 1));

        for (size_t y = 0; y < _height; ++y) {
            // Add filter byte (0 = no filter) at the start of each scanline
            rawData[y * (_width * bytesPerPixel + 1)] = 0;

            for (size_t x = 0; x < _width; ++x) {
                const RGBA& color = _data[y * _width + x];
                // Only save the blue channel as grayscale
                size_t pos = y * (_width * bytesPerPixel + 1) + 1 + x * bytesPerPixel;
                rawData[pos] = color.b;
            }
        }

        // zlib compression
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return false;
        }

        zs.next_in = rawData.data();
        zs.avail_in = (unsigned int)rawData.size();

        int ret;
        unsigned char outBuffer[4096]; // Use a buffer
        std::vector<unsigned char> compressedData;

        do {
            zs.next_out = outBuffer;
            zs.avail_out = sizeof(outBuffer);
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&zs);
                return false;
            }

            size_t have = sizeof(outBuffer) - zs.avail_out;
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + have);

        } while (zs.avail_out == 0);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return false;
        }

        // PNG Header and Data chunks.
        std::vector<unsigned char> pngData = {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a // PNG signature
        };

        // IHDR chunk
        {
            std::vector<unsigned char> ihdrData;
            ihdrData.reserve(13);
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_width >> (24 - i * 8)) & 0xFF);  // Width
            }
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_height >> (24 - i * 8)) & 0xFF); // Height
            }
            ihdrData.push_back(8);             // Bit depth
            ihdrData.push_back(0);             // Color type (0 = Grayscale)
            ihdrData.push_back(0);             // Compression method
            ihdrData.push_back(0);             // Filter method
            ihdrData.push_back(0);             // Interlace method

            // IHDR chunk length and type
            std::vector<unsigned char> ihdrChunk = createChunk("IHDR", ihdrData);
            pngData.insert(pngData.end(), ihdrChunk.begin(), ihdrChunk.end());
        }

        // IDAT chunk
        {
            std::vector<unsigned char> idatChunk = createChunk("IDAT", compressedData);
            pngData.insert(pngData.end(), idatChunk.begin(), idatChunk.end());
        }

        // IEND chunk
        {
           std::vector<unsigned char> iendChunk = createChunk("IEND", {});
           pngData.insert(pngData.end(), iendChunk.begin(), iendChunk.end());
        }

        // Write to file
        std::ofstream file(path.c_str(), std::ios::binary);
        if (!file) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(pngData.data()), pngData.size());
        return true;
    }

    /// @brief Saves the alpha channel of the image as a grayscale PNG file.
    /// @param path Destination file path.
    /// @return True on success, false on compression or I/O error.
    bool savePNGa(const td::String& path)
    {
        // PNG requires a filter byte at the start of each scanline
        const int bytesPerPixel = 1; // Grayscale - single channel

        // Prepare data with filter bytes (1 byte per scanline)
        std::vector<unsigned char> rawData(_height * (_width * bytesPerPixel + 1));

        for (size_t y = 0; y < _height; ++y) {
            // Add filter byte (0 = no filter) at the start of each scanline
            rawData[y * (_width * bytesPerPixel + 1)] = 0;

            for (size_t x = 0; x < _width; ++x) {
                const RGBA& color = _data[y * _width + x];
                // Only save the alpha channel as grayscale
                size_t pos = y * (_width * bytesPerPixel + 1) + 1 + x * bytesPerPixel;
                rawData[pos] = color.a;
            }
        }

        // zlib compression
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return false;
        }

        zs.next_in = rawData.data();
        zs.avail_in = (unsigned int)rawData.size();

        int ret;
        unsigned char outBuffer[4096]; // Use a buffer
        std::vector<unsigned char> compressedData;

        do {
            zs.next_out = outBuffer;
            zs.avail_out = sizeof(outBuffer);
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&zs);
                return false;
            }

            size_t have = sizeof(outBuffer) - zs.avail_out;
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + have);

        } while (zs.avail_out == 0);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return false;
        }

        // PNG Header and Data chunks.
        std::vector<unsigned char> pngData = {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a // PNG signature
        };

        // IHDR chunk
        {
            std::vector<unsigned char> ihdrData;
            ihdrData.reserve(13);
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_width >> (24 - i * 8)) & 0xFF);  // Width
            }
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_height >> (24 - i * 8)) & 0xFF); // Height
            }
            ihdrData.push_back(8);             // Bit depth
            ihdrData.push_back(0);             // Color type (0 = Grayscale)
            ihdrData.push_back(0);             // Compression method
            ihdrData.push_back(0);             // Filter method
            ihdrData.push_back(0);             // Interlace method

            // IHDR chunk length and type
            std::vector<unsigned char> ihdrChunk = createChunk("IHDR", ihdrData);
            pngData.insert(pngData.end(), ihdrChunk.begin(), ihdrChunk.end());
        }

        // IDAT chunk
        {
            std::vector<unsigned char> idatChunk = createChunk("IDAT", compressedData);
            pngData.insert(pngData.end(), idatChunk.begin(), idatChunk.end());
        }

        // IEND chunk
        {
           std::vector<unsigned char> iendChunk = createChunk("IEND", {});
           pngData.insert(pngData.end(), iendChunk.begin(), iendChunk.end());
        }

        // Write to file
        std::ofstream file(path.c_str(), std::ios::binary);
        if (!file) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(pngData.data()), pngData.size());
        return true;
    }

    /// @brief Saves the image as a 24-bit RGB PNG file.
    /// @param path Destination file path.
    /// @return True on success, false on compression or I/O error.
    bool savePNGrgb(const td::String& path)
    {
        // PNG requires a filter byte at the start of each scanline
        const int bytesPerPixel = 3; // RGB

        // Prepare data with filter bytes (1 byte per scanline)
        std::vector<unsigned char> rawData(_height * (_width * bytesPerPixel + 1));

        for (size_t y = 0; y < _height; ++y) {
            // Add filter byte (0 = no filter) at the start of each scanline
            rawData[y * (_width * bytesPerPixel + 1)] = 0;

            for (size_t x = 0; x < _width; ++x) {
                const RGBA& color = _data[y * _width + x];
                size_t pos = y * (_width * bytesPerPixel + 1) + 1 + x * bytesPerPixel;
                rawData[pos + 0] = color.r;
                rawData[pos + 1] = color.g;
                rawData[pos + 2] = color.b;
            }
        }

        // zlib compression
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return false;
        }

        zs.next_in = rawData.data();
        zs.avail_in = (unsigned int)rawData.size();

        int ret;
        unsigned char outBuffer[4096]; // Use a buffer
        std::vector<unsigned char> compressedData;

        do {
            zs.next_out = outBuffer;
            zs.avail_out = sizeof(outBuffer);
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&zs);
                return false;
            }

            size_t have = sizeof(outBuffer) - zs.avail_out;
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + have);

        } while (zs.avail_out == 0);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return false;
        }

        // PNG Header and Data chunks.
        std::vector<unsigned char> pngData = {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a // PNG signature
        };

        // IHDR chunk
        {
            std::vector<unsigned char> ihdrData;
            ihdrData.reserve(13);
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_width >> (24 - i * 8)) & 0xFF);  // Width
            }
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_height >> (24 - i * 8)) & 0xFF); // Height
            }
            ihdrData.push_back(8);             // Bit depth
            ihdrData.push_back(2);             // Color type (Truecolor)
            ihdrData.push_back(0);             // Compression method
            ihdrData.push_back(0);             // Filter method
            ihdrData.push_back(0);             // Interlace method

            // IHDR chunk length and type
            std::vector<unsigned char> ihdrChunk = createChunk("IHDR", ihdrData);
            pngData.insert(pngData.end(), ihdrChunk.begin(), ihdrChunk.end());
        }

        // IDAT chunk
        {
            std::vector<unsigned char> idatChunk = createChunk("IDAT", compressedData);
            pngData.insert(pngData.end(), idatChunk.begin(), idatChunk.end());
        }

        // IEND chunk
        {
           std::vector<unsigned char> iendChunk = createChunk("IEND", {});
           pngData.insert(pngData.end(), iendChunk.begin(), iendChunk.end());
        }

        // Write to file
        std::ofstream file(path.c_str(), std::ios::binary);
        if (!file) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(pngData.data()), pngData.size());
        return true;
    }

    /// @brief Saves the image as a 32-bit RGBA PNG file.
    /// @param path Destination file path.
    /// @return True on success, false on compression or I/O error.
    bool savePNGrgba(const td::String& path)
    {
        // PNG requires a filter byte at the start of each scanline
        const int bytesPerPixel = 4; // RGBA

        // Prepare data with filter bytes (1 byte per scanline)
        std::vector<unsigned char> rawData(_height * (_width * bytesPerPixel + 1));

        for (size_t y = 0; y < _height; ++y)
        {
            // Add filter byte (0 = no filter) at the start of each scanline
            rawData[y * (_width * bytesPerPixel + 1)] = 0;

            for (size_t x = 0; x < _width; ++x)
            {
                const RGBA& color = _data[y * _width + x];
                size_t pos = y * (_width * bytesPerPixel + 1) + 1 + x * bytesPerPixel;
                rawData[pos + 0] = color.r;
                rawData[pos + 1] = color.g;
                rawData[pos + 2] = color.b;

                // Fix: Ensure alpha is not zero when we actually have color
//                if (color.a == 0 && (color.r > 0 || color.g > 0 || color.b > 0))
//                {
//                    rawData[pos + 3] = 255; // Make fully opaque if colors exist but alpha is 0
//                }
//                else
//                {
//                    rawData[pos + 3] = color.a;
//                }
            }
        }

        // zlib compression
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return false;
        }

        zs.next_in = rawData.data();
        zs.avail_in = (unsigned int)rawData.size();

        int ret;
        unsigned char outBuffer[4096]; // Use a buffer
        std::vector<unsigned char> compressedData;

        do {
            zs.next_out = outBuffer;
            zs.avail_out = sizeof(outBuffer);
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_STREAM_ERROR) {
                deflateEnd(&zs);
                return false;
            }

            size_t have = sizeof(outBuffer) - zs.avail_out;
            compressedData.insert(compressedData.end(), outBuffer, outBuffer + have);

        } while (zs.avail_out == 0);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return false;
        }

        // PNG Header
        std::vector<unsigned char> pngData = {
            0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a
        };

        // IHDR chunk
        {
            std::vector<unsigned char> ihdrData;
            ihdrData.reserve(13);
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_width >> (24 - i * 8)) & 0xFF);
            }
            for (int i = 0; i < 4; i++) {
                ihdrData.push_back((_height >> (24 - i * 8)) & 0xFF);
            }
            ihdrData.push_back(8);             // Bit depth
            ihdrData.push_back(6);             // Color type (RGBA)
            ihdrData.push_back(0);             // Compression method
            ihdrData.push_back(0);             // Filter method
            ihdrData.push_back(0);             // Interlace method

            std::vector<unsigned char> ihdrChunk = createChunk("IHDR", ihdrData);
            pngData.insert(pngData.end(), ihdrChunk.begin(), ihdrChunk.end());
        }

        // IDAT chunk
        {
            std::vector<unsigned char> idatChunk = createChunk("IDAT", compressedData);
            pngData.insert(pngData.end(), idatChunk.begin(), idatChunk.end());
        }

        // IEND chunk
        {
            std::vector<unsigned char> iendChunk = createChunk("IEND", {});
            pngData.insert(pngData.end(), iendChunk.begin(), iendChunk.end());
        }

        // Write to file
        std::ofstream file(path.c_str(), std::ios::binary);
        if (!file) {
            return false;
        }
        file.write(reinterpret_cast<const char*>(pngData.data()), pngData.size());
        return true;
    }

    // Corrected helper function to create a PNG chunk with proper CRC calculation
    /// @brief Builds a well-formed PNG chunk with length, type, data, and CRC.
    /// @param type Four-character chunk type identifier (e.g. "IHDR").
    /// @param data Raw chunk data bytes.
    /// @return Byte vector containing the complete PNG chunk.
    std::vector<unsigned char> createChunk(const std::string& type, const std::vector<unsigned char>& data)
    {
        std::vector<unsigned char> chunk;
        std::uint32_t length = std::uint32_t(data.size());

        // Push the length (big-endian)
        for (int i = 0; i < 4; ++i) {
            chunk.push_back((length >> (24 - i * 8)) & 0xFF);
        }

        // Push the type
        for (char c : type) {
            chunk.push_back(static_cast<unsigned char>(c));
        }

        // Push the data
        chunk.insert(chunk.end(), data.begin(), data.end());

        // Calculate and push the CRC
        std::uint32_t crc = std::uint32_t(crc32(0, Z_NULL, 0));

        // Create a temporary buffer with type and data for CRC calculation
        std::vector<unsigned char> crcBuffer;
        crcBuffer.reserve(type.size() + data.size());

        // Add type to the CRC buffer
        for (char c : type) {
            crcBuffer.push_back(static_cast<unsigned char>(c));
        }

        // Add data to the CRC buffer
        crcBuffer.insert(crcBuffer.end(), data.begin(), data.end());

        // Calculate CRC on the combined buffer
        crc = std::uint32_t(crc32(crc, crcBuffer.data(), (uInt)crcBuffer.size()));

        // Add CRC to the chunk (big-endian)
        for (int i = 0; i < 4; ++i)
        {
            chunk.push_back((crc >> (24 - i * 8)) & 0xFF);
        }

        return chunk;
    }

public:

    /// @brief Default constructor; creates an empty image with zero dimensions.
    BMPImage() = default;

    /// @brief Constructs an image with the given dimensions; all pixels are default-initialised.
    /// @param width Image width in pixels.
    /// @param height Image height in pixels.
    BMPImage(std::size_t width, std::size_t height)
        : _data(width * height)
        , _width(static_cast<td::UINT4>(width))
        , _height(static_cast<td::UINT4>(height))
    {

    }

    /// @brief Sets a pixel using a float grayscale intensity and optional alpha.
    /// @param x Column index (0-based).
    /// @param y Row index (0-based).
    /// @param color Grayscale intensity in [0.0, 1.0].
    /// @param alpha Alpha value in [0.0, 1.0], defaults to 1.
    void set(std::int32_t x, std::int32_t y, float color, float alpha = 1)
    {
        if (!inBounds(y, x))
        {
            return;
        }
        auto& rgbPixel = _data[static_cast<std::size_t>(y) * _width + x];
        rgbPixel.r = std::floor(color*255+0.5f);
        rgbPixel.g = std::floor(color*255+0.5f);
        rgbPixel.b = std::floor(color*255+0.5f);
        rgbPixel.a = std::floor(alpha*255+0.5f);
    }

    /// @brief Sets a pixel using an RGBA colour value.
    /// @param x Column index (0-based).
    /// @param y Row index (0-based).
    /// @param color RGBA colour to assign.
    void set(std::int32_t x, std::int32_t y, const RGBA& color)
    {
        if (!inBounds(y, x))
        {
            return;
        }

        _data[static_cast<std::size_t>(y) * _width + x] = color;
    }

    /// @brief Returns the image width in pixels.
    /// @return Width of the image.
    td::UINT4 width() const
    {
        return _width;
    }

    /// @brief Returns the image height in pixels.
    /// @return Height of the image.
    td::UINT4 height() const
    {
        return _height;
    }

    /// @brief Saves the image to disk in the specified format.
    /// @param path Destination file path.
    /// @param type Output format; defaults to Type::BMPrgb.
    /// @return True on success, false on failure.
    bool save(const td::String& path, Type type = Type::BMPrgb)
    {
        switch (type)
        {
            case Type::BMPrgb:
                return saveBMPrgb(path);
            case Type::BMPrgba:
                return saveBMPrgba(path);
            case Type::PNGrgb:
                return savePNGrgb(path);
            case Type::PNGrgba:
                return savePNGrgba(path);
            //grayscale (single channel)
            case Type::PNGr:
                return savePNGr(path);
            case Type::PNGg:
                return savePNGg(path);
            case Type::PNGb:
                return savePNGb(path);
            case Type::PNGa:
                return savePNGa(path);
        }
        return false;
    }

    /// @brief Provides read-only access to the raw pixel data buffer.
    /// @return Const reference to the internal RGBA pixel vector.
    const cnt::SafeFullVector<RGBA>& getData() const
    {
        return _data;
    }
};

} //namespace cnt
