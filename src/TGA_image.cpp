//
// Created by Thomas Miller on 10/24/24.
//
#include <iostream>
#include "TGA_image.h"
#include <fstream>

//==============Header============

Image::Header::Header() {}
Image::Header::Header(std::ifstream& image_file) {
    image_file.read(reinterpret_cast<char*>(&idLength), sizeof(char));
    image_file.read(reinterpret_cast<char*>(&colorMapType), sizeof(char));
    image_file.read(reinterpret_cast<char*>(&dataTypeCode), sizeof(char));
    image_file.read(reinterpret_cast<char*>(&colorMapOrigin), sizeof(short));
    image_file.read(reinterpret_cast<char*>(&colorMapLength), sizeof(short));
    image_file.read(reinterpret_cast<char*>(&colorMapDepth), sizeof(char));
    image_file.read(reinterpret_cast<char*>(&xOrigin), sizeof(short));
    image_file.read(reinterpret_cast<char*>(&yOrigin), sizeof(short));
    image_file.read(reinterpret_cast<char*>(&width), sizeof(short));
    image_file.read(reinterpret_cast<char*>(&height), sizeof(short));
    image_file.read(reinterpret_cast<char*>(&bitsPerPixel), sizeof(char));
    image_file.read(reinterpret_cast<char*>(&imageDescriptor), sizeof(char));
}

bool Image::Header::write_header(std::ofstream& image_file) {
    if(!image_file.write(reinterpret_cast<char*>(&idLength), sizeof(char))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&colorMapType), sizeof(char))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&dataTypeCode), sizeof(char))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&colorMapOrigin), sizeof(short))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&colorMapLength), sizeof(short))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&colorMapDepth), sizeof(char))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&xOrigin), sizeof(short))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&yOrigin), sizeof(short))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&width), sizeof(short))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&height), sizeof(short))) return false;
    if(!image_file.write(reinterpret_cast<char*>(&bitsPerPixel), sizeof(char))) return false;
    if(!image_file.write(&imageDescriptor, sizeof(char))) return false;

    return true;
}
//Debugging purposes
void Image::Header::print_header() {
    std::cout << (int)idLength << std::endl;
    std::cout << (int)colorMapType << std::endl;
    std::cout << (int)dataTypeCode << std::endl;
    std::cout << (int)colorMapOrigin << std::endl;
    std::cout << (int)colorMapLength << std::endl;
    std::cout << (int)xOrigin << std::endl;
    std::cout << (int)yOrigin << std::endl;
    std::cout << (int)width << std::endl;
    std::cout << (int)height << std::endl;
    std::cout << (int)bitsPerPixel << std::endl;
    std::cout << (int)imageDescriptor << std::endl;
}
//================Pixel==================================
Image::Pixel::Pixel(){}

std::vector<unsigned char> Image::Pixel::Load(std::ifstream& image_file) {
    std::vector<unsigned char> bgr(3);
    image_file.read(reinterpret_cast<char*>(&bgr[0]), sizeof(unsigned char));
    image_file.read(reinterpret_cast<char*>(&bgr[1]), sizeof(unsigned char));
    image_file.read(reinterpret_cast<char*>(&bgr[2]), sizeof(unsigned char));

    return bgr;
}

//====================Image=========================
Image::Image() {}
void Image::setHeader(Header& header) {
    _header = header;
}

void Image::setPixels(std::vector<std::vector<unsigned char> >& pixels) {
    grid = pixels;
    num_Pixels = _header.width * _header.height;
}


Image::Image(std::string filePath) {
    std::ifstream image_file(filePath, std::ios::binary);
    if(!image_file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return;
    }
    Header temp(image_file);
    _header = temp;
    num_Pixels = _header.width * _header.height;
    grid.reserve(num_Pixels);
    for(int i = 0; i < num_Pixels; i++) {
        Pixel pixel;
        grid.push_back(pixel.Load(image_file));
    }
}

bool Image::write(std::string fileName) {
    std::ofstream new_image(fileName, std::ios::binary);
    if(!new_image.is_open()) {
        std::cerr << "Couldn't write to file" << std::endl;
        return false;
    }
//write the header info
    _header.write_header(new_image);
    // write the pixel info
    for(int i = 0; i < num_Pixels; i++) {
        new_image.write(reinterpret_cast<char*>(&grid[i][0]), sizeof(unsigned char));
        new_image.write(reinterpret_cast<char*>(&grid[i][1]), sizeof(unsigned char));
        new_image.write(reinterpret_cast<char*>(&grid[i][2]), sizeof(unsigned char));
    }

    new_image.close();
    return true;
}

void Image::combine_channels(Image& blue, Image& green, Image& red) {
    num_Pixels = _header.width * _header.height;
    std::vector<std::vector<unsigned char> > new_grid(num_Pixels);
    for(int i = 0; i < num_Pixels; i++) {
        std::vector<unsigned char> bgr(3);
        bgr[0] = blue.grid[i][0];
        bgr[1] = green.grid[i][1];
        bgr[2] = red.grid[i][2];
        new_grid[i] = bgr;
    }
    setPixels(new_grid);
}

void Image::rotate_180() {
    std::vector<std::vector<unsigned char> > flipped_grid(num_Pixels);
    // for(int i = 0; i < num_Pixels; i++) {
    //     flipped_grid[i].resize(3);
    // }
        for(int i = 0; i < num_Pixels; i++) {
            std::vector<unsigned char> rgb(3);
            rgb[0] = grid[num_Pixels - 1 - i][0];
            rgb[1] = grid[num_Pixels - 1 - i][1];
            rgb[2] = grid[num_Pixels - 1 - i][2];
            flipped_grid[i] = rgb;
        }

    grid = flipped_grid;
}
//============================================================================================
//=================ALGORITHMS=================================================================
//============================================================================================
Image Image::multiply(const Image& other) {
    Image result;
    result.setHeader(_header);
    num_Pixels = _header.width * _header.height;
    std::vector<std::vector<unsigned char> > new_grid(num_Pixels);
    for(int i = 0; i < num_Pixels; i++) {
        std::vector<unsigned char> temp(3);
        for(int j = 0; j < 3; j++) {
            float reduced = grid[i][j] / 255.0f;
            float reduced2 = other.grid[i][j] / 255.0f;
            int push_value = ((reduced * reduced2)*255) + 0.5f;
            temp[j] = static_cast<unsigned char>(push_value);
        }
        new_grid[i] = temp;
    }
    result.setPixels(new_grid);
    return result;
}

void Image::multiply(unsigned int index, int scale) {
    if(index > 2) return;
    for(int i = 0; i < num_Pixels; i++) {
        float reduced = grid[i][index] / 255.0f;
        reduced *= scale;
        reduced = reduced * 255 + 0.5f;
        if(reduced > 255) {
            reduced = 255;
        }
        if(reduced < 0) {
            reduced = 0;
        }
        grid[i][index] = static_cast<unsigned char>(reduced);
    }
}

Image Image::screen(const Image& other) {
    Image result;
    result.setHeader(_header);
    num_Pixels = _header.width * _header.height;
    std::vector<std::vector<unsigned char> > new_grid(num_Pixels);
    for(int i = 0; i < num_Pixels; i++) {
        std::vector<unsigned char> temp(3);
        for(int j = 0; j < 3; j++) {
            float reduced = grid[i][j] / 255.0f;
            float reduced2 = other.grid[i][j] / 255.0f;
            int push_value = 0.5f + (255 * (1 - (1 - reduced) * (1 - reduced2)));
            temp[j] = static_cast<unsigned char>(push_value);
        }
        new_grid[i] = temp;
    }
    result.setPixels(new_grid);
    return result;
}


Image Image::subtract(const Image& other) {
    Image result;
    result.setHeader(_header);
    result.num_Pixels = _header.width * _header.height;
    std::vector<std::vector<unsigned char> > new_grid(num_Pixels);
    for(int i = 0; i < num_Pixels; i++) {
        std::vector<unsigned char> temp(3);
        for(int j = 0; j < 3; j++) {
            int resultant = static_cast<int>(grid[i][j]) - static_cast<int>(other.grid[i][j]);
            if(resultant < 0) {
                resultant = 0;
            }
            temp[j] = static_cast<unsigned char>(resultant);
        }
        new_grid[i] = temp;
    }
    result.setPixels(new_grid);
    return result;
}

void Image::add(int color_index, int scale) {
    for(int i = 0; i < num_Pixels; i++) {
        int sum = static_cast<int>(grid[i][color_index]) + scale;
        if(sum > 255) {
            sum = 255;
        }
        grid[i][color_index] = static_cast<unsigned char>(sum);
    }

}

Image Image::overlay(const Image& other) {
    Image result;
    result.setHeader(_header);
    num_Pixels = _header.width * _header.height;
    std::vector<std::vector<unsigned char> > new_grid(num_Pixels);
    for(int i = 0; i < num_Pixels; i++) {
        std::vector<unsigned char> temp(3);
        for(int j = 0; j < 3; j++) {
            float reduced = grid[i][j] / 255.0f;
            float reduced2 = other.grid[i][j] / 255.0f;
            if(reduced2 > 0.5f) {
                int push_value = 0.5f + (255 * (1 - (2 * (1 - reduced) * (1 - reduced2))));
                temp[j] = static_cast<unsigned char>(push_value);
            }
            else {
                int push_value = 2 * reduced * reduced2 * 255 + 0.5f;
                temp[j] = static_cast<unsigned char>(push_value);
            }
        }
        new_grid[i] = temp;
    }
    result.setPixels(new_grid);
    return result;
}
//debugging purposes
void Image::print_values() const {
    for(int i = 0; i < 10; i++) {
        std::cout << (int)grid[i][0] << ", " << (int)grid[i][1] << ", " << (int)grid[i][2] << std::endl;
    }
}
//================================================
// ================BIG THREE======================
//================================================

Image::Image(const Image& other) {
    _header = other._header;
    num_Pixels = other.num_Pixels;
    for(int i = 0; i < num_Pixels; i++) {
        grid.push_back(other.grid[i]);
    }
}

Image& Image::operator=(const Image& other) {
    if(this == &other) {
        return *this;
    }
    _header = other._header;
    num_Pixels = other.num_Pixels;
    for(int i = 0; i < num_Pixels; i++) {
        grid.push_back(other.grid[i]);
    }
    return *this;
}

Image::~Image() {
    grid.clear();
}