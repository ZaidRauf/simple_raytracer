#include "imagebuffer.h"

ImageBuffer::ImageBuffer(const float width, const float height) : image_width(width), image_height(height), is_little_endian(check_little_endian()){
    image_buffer = std::make_unique<RGBA_Color[]>(image_width * image_height);

    RGBA_Color *img_buf_data = image_buffer.get();
    for(auto i = 0; i < image_width * image_height; i++){
        img_buf_data[i] = BACKGROUND_WHITE;
        // img_buf_data[i] = (1 << 24) | (129 << 16) | (127 << 8) | 0xFF;
    }
}


void ImageBuffer::set_pixel(const unsigned int x, const unsigned int y, const RGBA_Color color){
    if(x < 0 || y < 0 || x >= image_width || y >= image_height){
        return;
    }

    unsigned int image_pos = x + (y * image_width);

    image_buffer.get()[image_pos] = color;

    return;
}

RGBA_Color ImageBuffer::get_pixel(const unsigned int x, const unsigned int y){
    if(x < 0 || y < 0 || x >= image_width || y >= image_height){
        return BACKGROUND_WHITE;
    }

    unsigned int image_pos = x + (y * image_width);

    return image_buffer.get()[image_pos];
}

void ImageBuffer::dump_buffer(const std::string &filename){
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

const bool ImageBuffer::check_little_endian(){
    volatile int endian_check = 1;
    volatile bool is_little_endian = (*reinterpret_cast<volatile char*>(&endian_check) == 1);
    return is_little_endian;
}

void ImageBuffer::write_tga(const std::string &filename){   
    std::filebuf f;
    f.open(filename, std::ios::out | std::ios::binary);

    // Number of Characters in Image Identification Field
    f.sputc(0x00);

    // Color Map Not Included
    f.sputc(0x00);

    // Image Type Code 2 (Unmapped RGB)
    f.sputc(0x02);

    // No Color Map Included so pad 5 bytes with zeros
    for(auto i = 0; i < 5; i++){
        f.sputc(0x00);
    }

    // Image Specification
    uint16_t x_origin = 0x0000;
    f.sputn(reinterpret_cast<char*>(&x_origin), sizeof(uint16_t));

    uint16_t y_origin = 0x0000;
    f.sputn(reinterpret_cast<char*>(&y_origin), sizeof(uint16_t));

    uint16_t img_width = this->image_width;
    f.sputn(reinterpret_cast<char*>(&img_width), sizeof(uint16_t));

    uint16_t img_height = this->image_height;
    f.sputn(reinterpret_cast<char*>(&img_height), sizeof(uint16_t));

    // Bit Depth (32 Bits)
    f.sputc(0x20);

    // Image Descriptor Byte
    f.sputc(0b00101000);

    // Image ID field and Color Map data are skipped
    // Color Data

    RGBA_Color *img_buf_data = image_buffer.get();
    for(auto i = 0; i < image_width * image_height; i++){
        RGBA_Color orig =  img_buf_data[i];
        uint32_t argb_color = (orig << 24) | (orig >> 8);
        f.sputn(reinterpret_cast<char*>(&argb_color), sizeof(uint32_t));
    }

    f.close();
}
