# ============================================================
# Configuration
# ============================================================
BUILD      ?= debug
BUILD_DIR  := build/$(BUILD)

RAYLIB_SRC := vendor/raylib/src
CC         := gcc
CFLAGS     := -Wall -Wextra -std=c11 -I./utils -I./include -I./src -I$(RAYLIB_SRC) -D_GLFW_X11
LDFLAGS    := -lm


RAYLIB_OBJS    := $(BUILD_DIR)/raylib/rcore.o \
                  $(BUILD_DIR)/raylib/rshapes.o \
                  $(BUILD_DIR)/raylib/rtextures.o \
                  $(BUILD_DIR)/raylib/rtext.o \
                  $(BUILD_DIR)/raylib/rmodels.o \
                  $(BUILD_DIR)/raylib/raudio.o \
                  $(BUILD_DIR)/raylib/rglfw.o
RAYLIB_LIB     := $(BUILD_DIR)/libraylib.a

# Platform detection
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDFLAGS += -framework OpenGL -framework Cocoa -framework IOKit
endif
ifeq ($(UNAME), Linux)
    LDFLAGS += -lGL -lpthread -ldl -lrt -lX11
endif

# Debug vs Release
BUILD ?= debug
ifeq ($(BUILD), debug)
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif

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
	$(CC) -std=c11 -I$(RAYLIB_SRC) -I$(RAYLIB_SRC)/external/glfw/include -D_GLFW_X11 -DPLATFORM_DESKTOP -w -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(RAYLIB_LIB): $(RAYLIB_OBJS)
	ar rcs $@ $^

clean:
	rm -rf build/

run: all
	./$(TARGET)

debug: BUILD=debug
debug: all

release: BUILD=release
release: all

.PHONY: all clean run debug release
