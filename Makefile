# Paths to SDL3 files
SDL_INC = -IC:/deps/SDL3/include
SDL_LIB = -LC:/deps/SDL3/lib

# Compiler flags
CFLAGS = -Wall -std=c11 $(SDL_INC)
LDFLAGS = $(SDL_LIB) -lSDL3

# Build Target
all:
	gcc main.c -o main.exe $(CFLAGS) $(LDFLAGS)