
# Compiler

build:
	g++ -std=c++11 -o project2.out src/main.cpp src/TGA_image.cpp

tasks:
	build
	./project2.out part11.tga car.tga multiply layer1.tga
	./project2.out part12.tga layer1.tga flip
	./project2.out part13.tga layer2.tga subtract layer1.tga

