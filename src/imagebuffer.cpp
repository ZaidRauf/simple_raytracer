#include "imagebuffer.h"

ImageBuffer::ImageBuffer(const float width, const float height) : image_width(width), image_height(height){
    image_buffer = std::make_unique<RGBA_Color[]>(image_width * image_height);

    RGBA_Color *img_buf_data = image_buffer.get();
    for(auto i = 0; i < image_width * image_height; i++){
        img_buf_data[i] = BACKGROUND_WHITE;
    }
}


void ImageBuffer::SetPixel(const unsigned int x, const unsigned int y, const RGBA_Color color){
    if(x < 0 || y < 0 || x >= image_width || y >= image_height){
        return;
    }

    unsigned int image_pos = x + (y * image_width);

    image_buffer.get()[image_pos] = color;

    return;
}

RGBA_Color ImageBuffer::GetPixel(const unsigned int x, const unsigned int y){
    if(x < 0 || y < 0 || x >= image_width || y >= image_height){
        return BACKGROUND_WHITE;
    }

    unsigned int image_pos = x + (y * image_width);

    return image_buffer.get()[image_pos];
}

void ImageBuffer::DumpBuffer(const std::string &filename){
    std::ofstream file;
    file.open(filename);

    RGBA_Color *img_buf_data = image_buffer.get();

    file << "Width: " << image_width << std::endl;
    file << "Height: " << image_height << std::endl;

    for(auto i = 0; i < image_width * image_height; i++){
        file << img_buf_data[i] << std::endl;
    }

    file.close();
}
