.PHONY: all
all: lib/libglad.a

lib/libglad.a: include/glad/glad.h src/glad.c
	@echo "Building GLAD"
	@mkdir -p build
	@mkdir -p lib
	@$(CC) -Iinclude -c -o build/glad.o src/glad.c
	@ar rcs lib/libglad.a build/glad.o