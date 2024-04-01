TARGET := ocev-ga
SRC_EXT := .cpp
HDR_EXT := .h
OBJ_EXT := .o

BUILD_DIR := build
SRC_DIR := src
OBJ_DIR := obj

SRCS := $(shell find $(SRC_DIR) -name "*$(SRC_EXT)")
OBJS := $(addprefix $(OBJ_DIR)/,$(notdir $(patsubst %$(SRC_EXT),%$(OBJ_EXT),$(SRCS))))

CXX := g++
CXX_FLAGS := -g -Wall -std=c++11

all: $(BUILD_DIR)/$(TARGET)
	printf "POP=30\nDIM=100\nSEED=\nCROSSOVER_RATE=1\nMUTATION_RATE=0.03\nP_M=0.1\nINT_RANGE_START=-5\nINT_RANGE_END=10\nREAL_RANGE_START=-10\nREAL_RANGE_END=10\nGENERATIONS=5000" > $(BUILD_DIR)/conf.conf

$(BUILD_DIR)/$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) -o $@ $^
$(OBJ_DIR)/%$(OBJ_EXT): $(SRCS) | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -c -o $@ $(filter %$(basename $(notdir $@))$(SRC_EXT),$(SRCS))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

.PHONY: all clean