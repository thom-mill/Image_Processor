#include <iostream>
#include <fstream>
#include <vector>
#include "TGA_image.h"
#include <sys/stat.h>
#include <algorithm>
#include <cstring>

void task1() {
    Image layer1("src/input/layer1.tga");
    Image pattern1("src/input/pattern1.tga");
    Image resultOfTask1 = layer1.multiply(pattern1);
    resultOfTask1.write("output/part1.tga");
}

void task2() {
    Image car("src/input/car.tga");
    Image layer2("src/input/layer2.tga");
    Image task2 = car.subtract(layer2);
    task2.write("output/part2.tga");
}

void task3() {
    Image layer1("src/input/layer1.tga");
    Image pattern2("src/input/pattern2.tga");
    Image text("src/input/text.tga");
    Image blend = layer1.multiply(pattern2);
    Image resultOfTask3 = blend.screen(text);
    resultOfTask3.write("output/part3.tga");
}

void task4() {
    Image layer2("src/input/layer2.tga");
    Image circles("src/input/circles.tga");
    Image pattern2("src/input/pattern2.tga");
    Image combine = layer2.multiply(circles);
    Image resultOfTask4 = combine.subtract(pattern2);
    resultOfTask4.write("output/part4.tga");
}

void task5() {
    Image layer1("src/input/layer1.tga");
    Image pattern1("src/input/pattern1.tga");
    Image result = layer1.overlay(pattern1);
    result.write("output/part5.tga");
}

void task6() {
    Image car("src/input/car.tga");
    car.add(1, 200);
    car.write("output/part6.tga");
}

void task7() {
    Image car("src/input/car.tga");
    car.multiply(2, 4);
    car.multiply(0, 0);
    car.write("output/part7.tga");
}

void task8() {
    Image car("src/input/car.tga");
    Image redFile;
    Image greenFile;
    Image blueFile;

    redFile.setHeader(car._header);
    greenFile.setHeader(car._header);
    blueFile.setHeader(car._header);

    std::vector<std::vector<unsigned char> > bluePix(car.num_Pixels);
    std::vector<std::vector<unsigned char> > greenPix(car.num_Pixels);
    std::vector<std::vector<unsigned char> > redPix(car.num_Pixels);

    for(int i = 0; i < car.num_Pixels; i++) {
        std::vector<unsigned char> blue_vec(3);
        std::vector<unsigned char> green_vec(3);
        std::vector<unsigned char> red_vec(3);
        for(int j = 0; j < 3; j++) {
            blue_vec[j] = car.grid[i][0];
            green_vec[j] = car.grid[i][1];
            red_vec[j] = car.grid[i][2];
        }
        bluePix[i] = blue_vec;
        greenPix[i] = green_vec;
        redPix[i] = red_vec;
    }
    blueFile.setPixels(bluePix);
    greenFile.setPixels(greenPix);
    redFile.setPixels(redPix);

    redFile.write("output/part8_r.tga");
    greenFile.write("output/part8_g.tga");
    blueFile.write("output/part8_b.tga");
}

void task9() {
    Image blue("src/input/layer_blue.tga");
    Image green("src/input/layer_green.tga");
    Image red("src/input/layer_red.tga");
    Image combined;
    combined.setHeader(blue._header);
    combined.combine_channels(blue,green,red);
    combined.write("output/part9.tga");
}

void task10() {
    Image text2("src/input/text2.tga");
    text2.rotate_180();
    text2.write("output/part10.tga");
}
//make directory
bool directory_exists(const std::string& path) {
    struct stat info;
    return (stat(path.c_str(), &info) == 0 && info.st_mode & S_IFDIR);
}

void create_directory(const std::string& path) {
    mkdir(path.c_str(), S_IRWXU);
}


// ======COMMAND LINE INTERFACE=============
void help_message() {
    std::cout << "Project 2: Image Processing, Fall 2024" << std::endl << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << '\t' << "./project2.out [output] [firstImage] [method] [...]" << std::endl;
}

Image flow_control(std::string operation, Image& bottom, const Image& top = Image()) {
    if(operation == "multiply") {
        return bottom.multiply(top);
    }
    if(operation == "subtract") {
        return bottom.subtract(top);
    }
    if(operation == "overlay") {
        return bottom.overlay(top);
    }
    if(operation == "screen") {
        return bottom.screen(top);
    }
    if(operation == "flip") {
        bottom.rotate_180();
        return bottom;
    }
    if(operation == "onlyred") {
        bottom.only_red();
        return bottom;
    }
    if(operation == "onlygreen") {
        bottom.only_green();
        return bottom;
    }
    if(operation == "onlyblue") {
        bottom.only_blue();
        return bottom;
    }
    else {
        return bottom;
    }
}

Image scale_ops(std::string operation, Image& bottom, int scale) {
    if(operation == "addred") {
        bottom.add(2, scale);
        return bottom;
    }
    if(operation == "addgreen") {
        bottom.add(1, scale);
        return bottom;
    }
    if(operation == "addblue") {
        bottom.add(0, scale);
        return bottom;
    }
    if(operation == "scalered") {
        bottom.multiply(2, scale);
        return bottom;
    }
    if(operation == "scalegreen") {
        bottom.multiply(1, scale);
        return bottom;
    }
    if(operation == "scaleblue") {
        bottom.multiply(0, scale);
        return bottom;
    }
    else {
        return bottom;
    }

}

bool file_exists(const std::string& path) {
    struct stat info;
    // Use stat to get information about the path
    if (stat(path.c_str(), &info) != 0) {
        return false; // Path does not exist or error occurred
    }

    // Check if it is a regular file
    return (info.st_mode & S_IFREG) != 0;
}

std::string error_message(const std::string& filePath, bool type = 0) {
    if(type == 0){
        if(filePath.substr(filePath.size() - 4, 4) != ".tga") {
            return "Invalid file name.";
        }
        return "File does not exist." ;
    }
    else {
        if(filePath.substr(filePath.size() - 4, 4) != ".tga") {
            return "Invalid argument, invalid file name.";
        }
        return "Invalid argument, file does not exist." ;
    }
}

int main(int argc, char* argv[])
{

    if(!directory_exists("output")) {
        create_directory("output");
    }

    std::vector<std::string> args;
    for(int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    //handle help
    if(argc == 1 || args[1] == "--help") {
        help_message();
        return 0;
    }

    std::string output_name = args[1];
    if(output_name.size() <= 4 || output_name.substr(output_name.size() - 4, 4) != ".tga") {
        std::cerr << "Invalid file name." << std::endl;
        return 1;
    }

    std::string two_image_ops[4] = {"multiply", "subtract", "overlay", "screen"};
    std::string one_image_ops[4] = {"flip", "onlyred", "onlygreen", "onlyblue"};
    std::string integer_ops[6] = {"addred", "addgreen", "addblue", "scalered", "scalegreen", "scaleblue"};

    Image first;//initalize the base layer before entering the while loop scope
    if(file_exists(args[2])) {
        first = Image(args[2]);
    }
    else {
        std::cerr << error_message(args[2]) << std::endl;
        return 1;
    }

    int i = 3;
    while(i < argc) {
        //check if the operation requires two images
        if(std::find(std::begin(two_image_ops), std::end(two_image_ops), args[i]) != std::end(two_image_ops)) {
            if(argc < i + 2) { // check for a missing argument
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            if(file_exists(args[i+1])) {
                Image second_image(args[i+1]);
                first = flow_control(args[i], first, second_image);
                i+=2;
                continue;
            }
            std::cerr << error_message(args[i+1], 1) << std::endl;
            return 1;
        }
        //check if the operation requires no additional arguments
        else if(std::find(std::begin(one_image_ops), std::end(one_image_ops), args[i]) != std::end(one_image_ops)) {
            first = flow_control(args[i], first);
            i++;
            continue;
        }
        //check if the operation requires a scaler quantity (int)
        else if(std::find(std::begin(integer_ops), std::end(integer_ops), args[i]) != std::end(integer_ops)) {
            if(argc < i + 2) {
                std::cerr << "Missing argument." << std::endl;
                return 1;
            }
            try {
                int scaler = std::stoi(args[i+1]);
                scale_ops(args[i], first, scaler);
                i+=2;
                continue;

            }
            catch(std::invalid_argument) {
                std::cerr << "Invalid argument, expected number." << std::endl;
                return 1;
            }
        }
        else if(args[i] == "combine") {
            if(file_exists(args[i+1]) && file_exists(args[i+2])) {
                Image blue(args[i+2]);
                Image green(args[i+1]);
                first.combine_channels(blue, green, first);
                i += 3;
            }
            else {
                if(!file_exists(args[i+1]))
                    std::cerr << error_message(args[i+1], 1) << std::endl;
                if(!file_exists(args[i+2]))
                    std::cerr << error_message(args[i+2], 1) << std::endl;;
                return 1;
            }
        }
        else {
            std::cerr << "Invalid method name." << std::endl;
            return 1;
        }
    }
    first.write(args[1]); // write the final image
    return 0;
}
