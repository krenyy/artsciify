NAME=kronemar

CMNFLAGS=

CXX=g++
CXXFLAGS=-std=c++17 -c

LD=g++
LDFLAGS=-lstdc++fs -lpng

ifdef DEBUG
CMNFLAGS+=-fsanitize=address
CXXFLAGS+=-g -std=c++17 -Wall -Wextra -Werror -Wpedantic -Weffc++ -Wshadow -Wconversion -Wsign-conversion \
          -Wfloat-equal -Wunused -Wuninitialized -Woverloaded-virtual -Wformat -Wmissing-declarations \
          -Wcast-qual -Wnull-dereference
endif
ifdef OPTIMIZE
CXXFLAGS+=-O2
endif

TARGET=$(NAME)

SRC_DIR=src
BUILD_DIR=build
DOC_DIR=doc

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
SRCS+=$(wildcard $(SRC_DIR)/filters/*.cpp)
SRCS+=$(wildcard $(SRC_DIR)/filters/scaling/*.cpp)
SRCS+=$(wildcard $(SRC_DIR)/transforms/*.cpp)
SRCS+=$(wildcard $(SRC_DIR)/transforms/color_transforms/*.cpp)
SRCS+=$(wildcard $(SRC_DIR)/transforms/text_transforms/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

.PHONY: all clean compile create_build_dir run
.DEFAULT_GOAL=all

all: compile doc

clean:
	rm -rf $(TARGET) $(BUILD_DIR) $(DOC_DIR)

create_build_dir:
	@mkdir -p $(BUILD_DIR)

compile: $(TARGET)

doc:
	doxygen

run: compile
	./$(TARGET)

$(TARGET): $(OBJS) | create_build_dir
	$(LD) -o $(TARGET) $(CMNFLAGS) $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CMNFLAGS) $(CXXFLAGS) -o $@ $<


ifeq (,$(filter clean,$(MAKECMDGOALS)))
include $(DEPS)
endif


$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM -MQ $(patsubst $(BUILD_DIR)/%.d,$(BUILD_DIR)/%.o,$@) $< -MF $@
