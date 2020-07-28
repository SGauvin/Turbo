################################################################################
##### Variables and settings
################################################################################

# Project name
EXEC = libspdlog.a

# Build, bin and install directories (conserve root directories for clean)
BUILD_DIR_ROOT = build
BUILD_DIR := $(BUILD_DIR_ROOT)
BIN_DIR_ROOT = bin
BIN_DIR := $(BIN_DIR_ROOT)
INSTALL_DIR := ~/Desktop/$(EXEC)

# Libs path
TURBO_PATH=turbo
SPDLOG_PATH=libs/spdlog

# Sources (searches recursively inside the source directory)
SRC_DIR = src
SRCS = $(sort $(shell find $(SRC_DIR) -name '*.cpp'))

# Includes
INCLUDE_DIR = include
INCLUDES := -I$(INCLUDE_DIR) -idirafter$(TURBO_PATH)/include -idirafter$(SPDLOG_PATH)/include

# C preprocessor settings
CPPFLAGS := -MMD -MP $(INCLUDES)

# C++ compiler settings
CC = gcc
CXX = g++
CXXFLAGS = -std=c++17
WARNINGS = -Wall -Wpedantic -Wextra -Wconversion

# Linker flags
LDFLAGS =
LDFLAGS += -L$(TURBO_PATH)/$(BIN_DIR)

# Libraries to link
LDLIBS = -lturbo

# Target OS detection
ifeq ($(OS),Windows_NT) # OS is a preexisting environment variable on Windows
	OS = windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin)
		OS = macos
	else ifeq ($(UNAME),Linux)
		OS = linux
	else
    	$(error OS not supported by this Makefile)
	endif
endif

# OS-specific build and bin directories
BUILD_DIR := $(BUILD_DIR)/make_$(OS)
BIN_DIR := $(BIN_DIR)/$(OS)
ifeq ($(OS),windows)
	# Windows 32-bit
	ifeq ($(win32),1)
		BUILD_DIR := $(BUILD_DIR)32
		BIN_DIR := $(BIN_DIR)32
	# Windows 64-bit
	else
		BUILD_DIR := $(BUILD_DIR)64
		BIN_DIR := $(BIN_DIR)64
	endif
endif

# OS-specific compilation and linking settings
ifeq ($(OS),windows)
	# Disable console output on release builds
	ifeq ($(release),1)
		LDFLAGS += -mwindows
	endif

	# 32-bit flags
	ifeq ($(win32),1)
		CXXFLAGS += -m32
	# 64-bit flags
	else
		CXXFLAGS += -m64
	endif
endif

# Debug (default) and release modes settings
ifeq ($(release),1)
	BUILD_DIR := $(BUILD_DIR)/release
	BIN_DIR := $(BIN_DIR)/release
	CXXFLAGS += -O3
	CPPFLAGS += -DNDEBUG
else
	BUILD_DIR := $(BUILD_DIR)/debug
	BIN_DIR := $(BIN_DIR)/debug
	CXXFLAGS += -O0 -g
endif

# Objects and dependencies
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

################################################################################
##### Targets
################################################################################

.PHONY: all
all: $(BIN_DIR)/$(EXEC)

# Build executable
$(BIN_DIR)/$(EXEC): $(OBJS)
	@echo "Building librairy: $@"
	@mkdir -p lib
	@ar rcs lib/$(EXEC) $(OBJS)

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -DSPDLOG_COMPILED_LIB -c $< -o $@

# Include automatically generated dependencies
-include $(DEPS)


# Build and run
.PHONY: run
run: all
	@echo "Starting program: $(BIN_DIR)/$(EXEC)"
	@cd ./$(BIN_DIR); ./$(EXEC)

# Copy assets to bin directory for selected platform
.PHONY: copyassets
copyassets:
	@echo "Copying assets with script: $(COPY_ASSETS_SCRIPT)"
	@./scripts/$(COPY_ASSETS_SCRIPT) 2> /dev/null

# Made by Misha Krieger-Raynauld
