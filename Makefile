FILES := $(wildcard ./src/**/*.c ./src/*.c)

linux:
	clang $(FILES) -o main -lallegro -lallegro_image -lallegro_primitives -lm
	./main

win:
	clang -g $(FILES) -o main -lallegro -lallegro_image -lallegro_primitives
	./main.exe
