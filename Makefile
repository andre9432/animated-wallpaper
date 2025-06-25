CC=g++

all: hello

hello: main.cpp shaders.cpp glad.c
	$(CC) main.cpp glad.c shaders.cpp -L./lib -luser32 -lgdi32 -lopengl32 -I./include -o animated-wallpaper.exe

clear:
	rm animated-wallpaper.exe