# Paths to SDL3 files
SDL_INC = -IC:/deps/SDL3/include
SDL_LIB = -LC:/deps/SDL3/lib

# Compiler flags
CFLAGS = -Wall -std=c11 $(SDL_INC)
LDFLAGS = $(SDL_LIB) -lSDL3

# Source files
SRCS = main.c graphics_module.c math_module.c custom_scenes.c

# Build Target
all:
	gcc $(SRCS) -o main.exe $(CFLAGS) $(LDFLAGS)