#pragma once

#include <cstdint>
#include <fstream>
#include <memory>

// Of form RGBA where A is assumed to be 255;
using RGBA_Color = std::uint32_t;

class ImageBuffer {
    public:
        ImageBuffer(const float width, const float height);
        const float image_width;
        const float image_height;
        void set_pixel(const unsigned int x, const unsigned int y, const RGBA_Color color);
        RGBA_Color get_pixel(const unsigned int x, const unsigned int y);
        void dump_buffer(const std::string &filename);
        void write_tga(const std::string &filename);

    private:
        std::unique_ptr<RGBA_Color[]> image_buffer;
        const static RGBA_Color BACKGROUND_WHITE = 0xFFFFFFFF;
        const static RGBA_Color BACKGROUND_BLACK = 0x000000FF;
        const bool check_little_endian();
        const bool is_little_endian;
};