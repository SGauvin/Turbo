################################################################################
##### Variables and settings
################################################################################

# Project name
EXEC = libimgui.a

# Build, bin and install directories (conserve root directories for clean)
BUILD_DIR_ROOT = build
BUILD_DIR := build

# Sources (searches recursively inside the source directory)
SRC_DIR = src
SRCS := $(sort $(shell find $(SRC_DIR) -name '*.cpp'))

# Includes
INCLUDE_DIR = include/imgui
INCLUDES := -I$(INCLUDE_DIR) -idirafter../GLFW/include -idirafter../glad/include

# C preprocessor settings
CPPFLAGS := -MMD -MP $(INCLUDES)

# C++ compiler settings
CXX = g++
CXXFLAGS = -std=c++17
WARNINGS =

# Linker flags
LDFLAGS =

# Libraries to link
LDLIBS =

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

# OS-specific compilation and linking settings
ifeq ($(OS),windows)
	# Add .exe extension to executable
	EXEC := $(EXEC)

	# Link everything statically on Windows (including libgcc and libstdc++)
	LDFLAGS += -static

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

CXXFLAGS += -O3
CPPFLAGS += -DNDEBUG

# Objects and dependencies
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

################################################################################
##### Targets
################################################################################

.PHONY: all
all: lib/$(EXEC)

# Build executable
lib/$(EXEC): $(OBJS)
	@echo "Building executable: $@"
	@mkdir -p $(@D)
	@ar rcs $@ $^

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(WARNINGS) -c $< -o $@

# Include automatically generated dependencies
-include $(DEPS)

# Install packaged program
.PHONY: install
install: all copyassets
	@echo "Packaging program to $(INSTALL_DIR)"
	@mkdir -p $(INSTALL_DIR); cp -r $(BIN_DIR)/. $(INSTALL_DIR)

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

# Clean build and bin directories for all platforms
.PHONY: clean
clean:
	@echo "Cleaning $(BUILD_DIR_ROOT) and $(BIN_DIR_ROOT) directories"
	@$(RM) -r $(BUILD_DIR_ROOT)
	@$(RM) -r lib

# Clean all assets from bin directories for all platforms
.PHONY: cleanassets
cleanassets:
	@echo "Cleaning assets for all platforms"
	@./scripts/clean_assets.sh

# Run clang-format on source code
.PHONY: format
format:
	@echo "Running clang-format"
	@clang-format -i $$(find $(SRC_DIR) $(INCLUDE_DIR) -name '*.cpp' -o -name '*.h' -o -name '*.inl')

# Generate documentation with Doxygen
.PHONY: doc
doc:
	@echo "Generating documentation"
	@doxygen doc/Doxyfile

# Print help information
.PHONY: help
help:
	@printf "\
	Usage: make target... [options]...\n\
	\n\
	Targets:\n\
	  all             Build executable (debug mode by default) (default target)\n\
	  install         Install packaged program to desktop (debug mode by default)\n\
	  run             Build and run executable (debug mode by default)\n\
	  copyassets      Copy assets to executable directory for selected platform and configuration\n\
	  clean           Clean build and bin directories (all platforms)\n\
	  cleanassets     Clean assets from executable directories (all platforms)\n\
	  format          Run clang-format on source code\n\
	  doc             Generate documentation with Doxygen\n\
	  help            Print this information\n\
	  printvars       Print Makefile variables for debugging\n\
	\n\
	Options:\n\
	  release=1       Run target using release configuration rather than debug\n\
	  win32=1         Build for 32-bit Windows (valid when built on Windows only)\n\
	\n\
	Note: the above options affect all, install, run, copyassets, and printvars targets\n"

# Print Makefile variables
.PHONY: printvars
printvars:
	@printf "\
	OS: $(OS)\n\
	EXEC: $(EXEC)\n\
	BUILD_DIR: $(BUILD_DIR)\n\
	BIN_DIR: $(BIN_DIR)\n\
	INSTALL_DIR: $(INSTALL_DIR)\n\
	SRC_DIR: $(SRC_DIR)\n\
	SRCS: $(SRCS)\n\
	INCLUDE_DIR: $(INCLUDE_DIR)\n\
	INCLUDES: $(INCLUDES)\n\
	CXX: $(CXX)\n\
	CPPFLAGS: $(CPPFLAGS)\n\
	CXXFLAGS: $(CXXFLAGS)\n\
	LDFLAGS: $(LDFLAGS)\n\
	LDLIBS: $(LDLIBS)\n\
	COPY_ASSETS_SCRIPT: $(COPY_ASSETS_SCRIPT)\n"

# Made by Misha Krieger-Raynauld