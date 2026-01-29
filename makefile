all:
	g++ source/main.cpp -Wextra -Wall -fsanitize=address -o ../civilization-debug.elf -g -lSDL3 -lGL -lGLEW -lm
	g++ source/main.cpp -Wextra -Wall -o ../civilization.elf -g -lSDL3 -lGL -lGLEW -lm
