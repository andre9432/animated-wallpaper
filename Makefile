CC=g++

all: hello

hello: ./src/main.cpp ./src/shaders.cpp ./src/glad.c
	$(CC) ./src/main.cpp ./src/glad.c ./src/shaders.cpp -L./lib -luser32 -lgdi32 -lopengl32 -I./include -o animated-wallpaper.exe

clear:
	rm animated-wallpaper.exe