# Compiler settings
CC := g++

# Compiler flags
CFLAGS := -Iinclude -std=c++20 -Wall $(shell sdl2-config --cflags)
# Linker flags
LDFLAGS := $(shell sdl2-config --libs) -lSDL2_ttf

# Base directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Find all source files in SRC_DIR and subdirectories
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
# Generate a list of object files by replacing the source file paths
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Your final executable
TARGET := $(BIN_DIR)/snake

# Default make
all: $(TARGET)

# Rule for making the executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $^ $(LDFLAGS) -o $@

# Pattern rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule for cleaning up
clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

