FILES := $(wildcard ./src/**/*.c ./src/*.c)

default:
	clang $(FILES) -o main -lallegro -lallegro_image
	./main