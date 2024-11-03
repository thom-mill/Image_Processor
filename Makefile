
# Compiler

build:
	g++ -std=c++11 -o project2.out src/main.cpp src/TGA_image.cpp

tasks: build
	./project2.out output/part11.tga src/input/car.tga multiply src/input/layer1.tga
	./project2.out output/part12.tga src/input/layer1.tga flip
	./project2.out output/part13.tga src/input/layer2.tga subtract src/input/layer1.tga

