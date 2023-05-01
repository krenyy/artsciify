NAME=artsciify

CXX=g++
CXXFLAGS=-c -std=c++17 -Wall -Wextra -Werror -Wpedantic

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
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all clean compile create_build_dir run
.DEFAULT_GOAL=all

all: compile

clean:
	rm -rf Makefile.d $(TARGET) $(BUILD_DIR)

create_build_dir:
	mkdir -p $(BUILD_DIR)

compile: create_build_dir $(OBJS)
	$(LD) -o $(TARGET) $(LDFLAGS) $(OBJS)

run: compile
	./$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -o $@ $<

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

include Makefile.d

Makefile.d: $(SRCS)
	rm -f $@
	$(foreach src,$(SRCS),$(CXX) -MM -MQ $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(src)) $(src) >> $@;)
