FILES := $(wildcard ./src/**/*.c ./src/*.c)

linux:
	clang -g $(FILES) -o main -lallegro -lallegro_image -lallegro_primitives -lm -std=c99
	./main

win:
	clang -g $(FILES) -o main -IC:/usr-headers -lallegro -lallegro_image -lallegro_primitives -std=c99
	./main.exe
