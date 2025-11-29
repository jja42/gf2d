##############################################################################
# Unified Emscripten Makefile for gf2d (corrected paths)
##############################################################################

PROJECT = gf2d
CC = emcc

# Include paths
INC_PATHS = ../include ../gfc/include ../gfc/simple_logger/include ../gfc/simple_json/include
INC_PARAMS = $(foreach d,$(INC_PATHS),-I$d)

# Project sources
SRC = audio.c camera.c enemy.c game.c gf2d_draw.c gf2d_entity.c gf2d_graphics.c \
      gf2d_shape.c gf2d_sprite.c level.c menu.c player.c touch.c

# Libraries
LIB_PATH = ../gfc
LIBS = $(LIB_PATH)/libs/libgfc.a $(LIB_PATH)/simple_json/libs/libsj.a $(LIB_PATH)/simple_logger/libs/libsl.a

# SDL / Emscripten flags
SDL_FLAGS = -sUSE_SDL=2 \
            -sUSE_SDL_IMAGE=2 \
			-sSDL2_IMAGE_FORMATS='["png"]' \
            -sUSE_SDL_TTF=2 \
            -sUSE_SDL_MIXER=2 \
			-sUSE_OGG=1	\
			-sALLOW_MEMORY_GROWTH=1	\
            --preload-file ../themes \
            --preload-file ../sfx \
			--preload-file ../fonts \
			--preload-file ../images \
			--preload-file ../levels \
            -lm

# Compiler flags
CFLAGS = -g -Wall -pedantic -std=gnu99 -Wno-unknown-pragmas -Wno-variadic-macros

# Phony targets
.PHONY: all libs gfc sj sl clean

# Default target
all: $(PROJECT).html

# Build main project
$(PROJECT).html: libs $(SRC)
	$(CC) $(CFLAGS) $(INC_PARAMS) $(SRC) $(LIBS) $(SDL_FLAGS) -o ../build/index.html

# Build all libraries first
libs: gfc sj sl

gfc:
	$(MAKE) -C ../gfc/src libgfc.a

sj:
	$(MAKE) -C ../gfc/simple_json/src libsj.a

sl:
	$(MAKE) -C ../gfc/simple_logger/src libsl.a

# Clean everything
clean:
	rm -f *.o *.html *.js *.wasm
	$(MAKE) -C ../gfc/src clean
	$(MAKE) -C ../gfc/simple_json/src clean
	$(MAKE) -C ../gfc/simple_logger/src clean