NAME=artsciify

CXX=g++
CXXFLAGS=-c -std=c++17 -Wall -Wextra -Werror -Wpedantic -Weffc++

LD=g++
LDFLAGS=-lpng

ifdef DEBUG
	CXXFLAGS+=-g -fsanitize=address
	LDFLAGS+=-fsanitize=address
endif

TARGET=$(NAME)

SRC_DIR=src
BUILD_DIR=build

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
SRCS+=$(wildcard $(SRC_DIR)/filters/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

.PHONY: all clean compile create_build_dir run
.DEFAULT_GOAL=all

all: compile

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

create_build_dir:
	@mkdir -p $(BUILD_DIR)

compile: $(TARGET)

run: compile
	./$(TARGET)

$(TARGET): $(OBJS) | create_build_dir
	$(LD) -o $(TARGET) $(LDFLAGS) $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

include $(DEPS)

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM -MQ $(patsubst $(BUILD_DIR)/%.d,$(BUILD_DIR)/%.o,$@) $< -MF $@
