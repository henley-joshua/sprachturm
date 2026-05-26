# ============================================================
# Configuration
# ============================================================
BUILD      ?= debug
BUILD_DIR  := build/$(BUILD)

RAYLIB_SRC := vendor/raylib/src
CC_LINUX   := gcc
CC_WINDOWS := x86_64-w64-mingw32-gcc
CC_MAC	   := clang

UNAME      := $(shell uname)
ifeq ($(UNAME), Linux)
	CC             := $(CC_LINUX)
	PLATFORM_FLAGS := -D_GLFW_X11 -DPLATFORM_DESKTOP
	LDFLAGS        := -lm -lGL -lpthread -ldl -lrt -lX11
	TARGET         := $(BUILD_DIR)/bin/game
endif

ifeq ($(OS), Windows_NT)
	CC             := $(CC_LINUX)
	PLATFORM_FLAGS := -DPLATFORM_DESKTOP
	LDFLAGS        := -lm -lopengll32 -lgdi32 -lwinmm
	TARGET         := $(BUILD_DIR)/bin/game.exe
endif

ifeq ($(UNAME), Darwin)
	CC			   := $(CC_MAC)
	PLATFORM_FLAGS := -DPLATFORM_DESKTOP -D_GLFW_COCOA
	EXTRA_FLAGS	   := -x objective-c
	LDFLAGS		   := -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	TARGET 		   := $(BUILD_DIR)/bin/game
else 
	EXTRA_FLAGS :=
endif


CFLAGS     := -Wall -Wextra -std=c11 \
	      -I./game -I./include -I./src -I$(RAYLIB_SRC) \
	      -D_GLFW_X11 \
	      $(PLATFORM_FLAGS)

ifeq ($(BUILD), debug)
	CFLAGS += -g -O0 -DDEBUG
else
	CFLAGS += -O2 -DNEBUG
endif

SRC_DIR := src
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

RAYLIB_OBJS    := $(BUILD_DIR)/raylib/rcore.o \
                  $(BUILD_DIR)/raylib/rshapes.o \
                  $(BUILD_DIR)/raylib/rtextures.o \
                  $(BUILD_DIR)/raylib/rtext.o \
                  $(BUILD_DIR)/raylib/rmodels.o \
                  $(BUILD_DIR)/raylib/raudio.o \
                  $(BUILD_DIR)/raylib/rglfw.o
RAYLIB_LIB     := $(BUILD_DIR)/libraylib.a

# ============================================================
# Source Discovery
# ============================================================
SRC_DIR  := src
SRCS     := $(shell find $(SRC_DIR) -name '*.c')
OBJS     := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET   := $(BUILD_DIR)/bin/game

# ============================================================
# Rules
# ============================================================
all: $(TARGET)

$(TARGET): $(OBJS) $(RAYLIB_LIB)
	@mkdir -p $(dir $@)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/raylib/%.o: $(RAYLIB_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) -std=c11 $(EXTRA_FLAGS) -I$(RAYLIB_SRC) -I$(RAYLIB_SRC)/external/glfw/include $(PLATFORM_FLAGS) -w -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(RAYLIB_LIB): $(RAYLIB_OBJS)
	ar rcs $@ $^

clean:
	rm -rf build/

clean-releases:
	rm -rf build/release/

run: all
	./$(TARGET)

debug: BUILD=debug
debug: all

release-linux:
	./scripts/release.sh linux

release-windows:
	./scripts/release.sh windows

release:
	./scripts/release.sh all

.PHONY: all clean clean-releases run debug \
	release-linux release-windows release
