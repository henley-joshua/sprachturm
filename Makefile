# ============================================================
# Configuration
# ============================================================
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -I./include -I./src
LDFLAGS  := -lraylib -lm

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
BUILD_DIR := build/$(BUILD)

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
TARGET := $(BUILD_DIR)/game

# ============================================================
# Rules
# ============================================================
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/

run: all
	./$(TARGET)

debug: BUILD=debug
debug: all

release: BUILD=release
release: all

.PHONY: all clean run debug release
