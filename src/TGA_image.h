//
// Created by Thomas Miller on 10/24/24.
//
#include <fstream>
#include <vector>
#ifndef IMAGE_HEADER_H
#define IMAGE_HEADER_H

#endif //IMAGE_HEADER_H
struct Image {
    struct Header {
        char idLength;
        char colorMapType;
        char dataTypeCode;
        short colorMapOrigin;
        short colorMapLength;
        char colorMapDepth;
        short xOrigin;
        short yOrigin;
        short width;
        short height;
        char bitsPerPixel;
        char imageDescriptor;
        Header();
        Header(std::ifstream& image_file);
        bool write_header(std::ofstream& image_file);
        void print_header();
    };

    struct Pixel {
        Pixel();
        std::vector<unsigned char> Load(std::ifstream& image_file);
    };

//=========image attributes=============
    std::vector<std::vector<unsigned char> > grid;
    Header _header;
    int num_Pixels;
    Image();
    void setHeader(Header& header);
    void setPixels(std::vector<std::vector<unsigned char> >& pixels);
    Image(std::string filePath);

    bool write(std::string fileName);
    void print_values() const;
    void combine_channels(Image& blue, Image& green, Image& red);
    void rotate_180();
    // =============ALGORITHMS====================
    Image multiply(const Image& other);
    void multiply(unsigned int index, int scale);
    Image screen(const Image& other);
    Image subtract(const Image& other);
    void add(int color_index, int scale);
    Image overlay(const Image& other);

    //BIG 3
    Image(const Image& other);
    Image& operator=(const Image& other);
    ~Image();
};