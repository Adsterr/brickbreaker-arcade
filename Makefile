# Compiler
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -g

# Directories
SRC_DIR := src
BUILD_DIR := build/objects
INCLUDE_DIR := $(SRC_DIR)/include

# Source files and object files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
HDR_FILES := $(wildcard $(INCLUDE_DIR)/*.hpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# SDL2 Flags
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LDFLAGS := $(shell sdl2-config --libs)

# SDL_ttf Flags
SDL_TTF_CFLAGS := $(shell pkg-config --cflags SDL2_ttf)
SDL_TTF_LDFLAGS := $(shell pkg-config --libs SDL2_ttf)

# Output binary
TARGET := game

# Build rules
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(SDL2_LDFLAGS) $(SDL_TTF_LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDR_FILES) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) $(SDL_TTF_CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
