FILES := $(wildcard ./src/**/*.c ./src/*.c)

linux:
	clang $(FILES) -o main -lallegro -lallegro_image
	./main

win:
	clang $(FILES) -o main -lallegro -lallegro_image
	./main.exe
