CPP_VER = c++23
COMPILER_OPTIONS := -g -Wall -Wno-deprecated -Wno-unused-but-set-variable
INCLUDES := -Iinclude -Isrc

ifeq ($(OS), Windows_NT)
	LINKS := -Llib/win64
	EXTRA_ARGS = -lglfw3dll 
else 
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Darwin)
		LINKS := -Llib/mac
		EXTRA_ARGS = lib/mac/libglfw.3.4.dylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -framework CoreFoundation
	else
		ERROR := $(error OS is not configured in the Makefile)
	endif
endif

SRC_FILES = $(shell find src -name "*.cpp" -o -name "*.c")
OUT_FILE = out/app

build:
	mkdir -p out
	g++-14 -std=$(CPP_VER) $(COMPILER_OPTIONS) $(INCLUDES) $(LINKS) $(SRC_FILES) $(EXTRA_ARGS) -o $(OUT_FILE)

clean:
	rm app