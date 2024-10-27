#include <iostream>
#include <fstream>
#include <vector>
#include "TGA_image.h"

void task1() {
    Image layer1("input/layer1.tga");
    Image pattern1("input/pattern1.tga");
    Image resultOfTask1 = layer1.multiply(pattern1);
    resultOfTask1.write("output/part1.tga");
}

void task2() {
    Image car("input/car.tga");
    Image layer2("input/layer2.tga");
    Image task2 = car.subtract(layer2);
    task2.write("output/part2.tga");
}

void task3() {
    Image layer1("input/layer1.tga");
    Image pattern2("input/pattern2.tga");
    Image text("input/text.tga");
    Image blend = layer1.multiply(pattern2);
    Image resultOfTask3 = blend.screen(text);
    resultOfTask3.write("output/part3.tga");
}

void task4() {
    Image layer2("input/layer2.tga");
    Image circles("input/circles.tga");
    Image pattern2("input/pattern2.tga");
    Image combine = layer2.multiply(circles);
    Image resultOfTask4 = combine.subtract(pattern2);
    resultOfTask4.write("output/part4.tga");
}

void task5() {
    Image layer1("input/layer1.tga");
    Image pattern1("input/pattern1.tga");
    Image result = layer1.overlay(pattern1);
    result.write("output/part5.tga");
}

void task6() {
    Image car("input/car.tga");
    car.add(1, 200);
    car.write("output/part6.tga");
}

void task7() {
    Image car("input/car.tga");
    car.multiply(2, 4);
    car.multiply(0, 0);
    car.write("output/part7.tga");
}

void task8() {
    Image car("input/car.tga");
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
    redFile.setPixels(bluePix);
    greenFile.setPixels(greenPix);
    blueFile.setPixels(redPix);

    redFile.write("output/part8_r.tga");
    greenFile.write("output/part8_g.tga");
    blueFile.write("output/part8_b.tga");
}

void task9() {
    Image blue("input/layer_blue.tga");
    Image green("input/layer_green.tga");
    Image red("input/layer_red.tga");
    Image combined;
    combined.setHeader(blue._header);
    combined.combine_channels(blue,green,red);
    combined.write("output/part9.tga");
}

void task10() {
    Image text2("input/text2.tga");
    text2.rotate_180();
    text2.write("output/part10.tga");
}

int main()
{
    task1();
    task2();
    task3();
    task4();
    task5();
    task6();
    task7();
    task8();
    task9();
    task10();
    return 0;
}
