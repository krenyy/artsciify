NAME=artsciify

CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror -Wpedantic -g -fsanitize=address -lpng

TARGET=$(NAME)

SRC_DIR=src
BUILD_DIR=build

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean compile
.DEFAULT_GOAL=all

all: compile

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

create_build_dir:
	mkdir -p $(BUILD_DIR)

compile: create_build_dir $(BUILD_DIR)/main.o
	$(CXX) -o $(TARGET) $(CXXFLAGS) $(OBJS)

run: compile
	./$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(BUILD_DIR)/image.o $(BUILD_DIR)/styles.o $(BUILD_DIR)/filters.o
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BUILD_DIR)/styles.o: $(BUILD_DIR)/image.o
