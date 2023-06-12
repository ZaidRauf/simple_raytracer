#pragma once

#include <cstdint>
#include <memory>
#include <fstream>

// Of form RGBA where A is assumed to be 255;
using RGBA_Color = uint32_t;

class ImageBuffer {
    public:
        ImageBuffer(const float width, const float height);
        const float image_width;
        const float image_height;

        void SetPixel(const unsigned int x, const unsigned int y, const RGBA_Color color);
        RGBA_Color GetPixel(const unsigned int x, const unsigned int y);

        void DumpBuffer(const std::string &filename);

    private:
        std::unique_ptr<RGBA_Color[]> image_buffer;
        const static RGBA_Color BACKGROUND_WHITE = 0xFFFFFFFF;
        const static RGBA_Color BACKGROUND_BLACK = 0x000000FF;
};