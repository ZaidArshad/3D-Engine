CPP_VER = c++20
COMPILER_OPTIONS := -g -Wall -Wno-deprecated -Wno-unused-but-set-variable
INCLUDES := -Iinclude -Isrc
OUT_DIR = out
SRD_DIR = src

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

SRC_CPP = $(shell find src -name "*.cpp")
SRC_C = $(shell find src -name "*.c" )
OBJS := $(SRC_CPP:%.cpp=%.o) $(SRC_C:%.c=%.o)
OBJECTS =$(patsubst %, out/%, $(OBJS))

all: $(OBJECTS)
	g++ -std=$(CPP_VER) $(COMPILER_OPTIONS) $(LINKS) $(OBJECTS) $(EXTRA_ARGS) -o out/app

out/%.o: %.cpp
	mkdir -p $(@D)
	g++ -std=$(CPP_VER) $(COMPILER_OPTIONS) $(INCLUDES) -c $< -o $@

out/%.o: %.c
	mkdir -p $(@D)
	g++ -std=$(CPP_VER) $(COMPILER_OPTIONS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf out/src
	rm app.exe
	rm app