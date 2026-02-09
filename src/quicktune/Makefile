##
## @file Makefile
## @brief QuickTune Build Configuration
##
## Makefile for building QuickTune module for STM32H562.
## Can be integrated into larger project or used standalone.
##
## @author DSP Team (Implementation Agent)
## @date 2026-02-09
## @target STM32H562 (Cortex-M33, 250 MHz)
##
## CONFIDENTIAL - Binary-only delivery
##

# ==============================================================================
# PROJECT CONFIGURATION
# ==============================================================================

PROJECT = quicktune
TARGET = $(PROJECT).elf
BINARY = $(PROJECT).bin

# ==============================================================================
# TOOLCHAIN
# ==============================================================================

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)as
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
SIZE = $(PREFIX)size

# ==============================================================================
# SOURCE FILES
# ==============================================================================

# QuickTune sources
SOURCES_CPP = \
	quicktune.cpp \
	eq10.cpp

# Example (optional, comment out for production)
# SOURCES_CPP += quicktune_example.cpp

# ==============================================================================
# INCLUDE PATHS
# ==============================================================================

INCLUDES = \
	-I. \
	-I../../CMSIS/Include \
	-I../../CMSIS/DSP/Include \
	-I../../Device/STM32H5xx/Include

# ==============================================================================
# COMPILER FLAGS
# ==============================================================================

# CPU architecture
CPU = -mcpu=cortex-m33
FPU = -mfpu=fpv5-sp-d16
FLOAT_ABI = -mfloat-abi=hard

# Instruction set
ARCH = -mthumb

# Optimization
OPT = -O3 -flto

# Defines
DEFINES = \
	-DSTM32H562xx \
	-DARM_MATH_CM33 \
	-DUSE_HAL_DRIVER

# Common flags
COMMON_FLAGS = \
	$(CPU) \
	$(FPU) \
	$(FLOAT_ABI) \
	$(ARCH) \
	$(OPT) \
	$(DEFINES) \
	$(INCLUDES)

# C++ flags
CXXFLAGS = \
	$(COMMON_FLAGS) \
	-std=c++11 \
	-fno-exceptions \
	-fno-rtti \
	-fno-threadsafe-statics \
	-ffunction-sections \
	-fdata-sections \
	-Wall \
	-Wextra \
	-Wno-unused-parameter

# ==============================================================================
# LINKER FLAGS
# ==============================================================================

LDFLAGS = \
	$(CPU) \
	$(FPU) \
	$(FLOAT_ABI) \
	$(ARCH) \
	-specs=nano.specs \
	-specs=nosys.specs \
	-Wl,--gc-sections \
	-Wl,-Map=$(PROJECT).map

# ==============================================================================
# LIBRARIES
# ==============================================================================

# CMSIS-DSP library (prebuilt)
LIBS = \
	-L../../CMSIS/DSP/Lib \
	-larm_cortexM33l_math \
	-lm

# ==============================================================================
# BUILD RULES
# ==============================================================================

# Object files
OBJECTS = $(SOURCES_CPP:.cpp=.o)

# Default target
all: $(TARGET) $(BINARY) size

# Link
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CXX) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

# Compile C++
%.o: %.cpp
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create binary
$(BINARY): $(TARGET)
	@echo "Creating binary..."
	$(OBJCOPY) -O binary $< $@

# Show size
size: $(TARGET)
	@echo "Size information:"
	$(SIZE) $(TARGET)

# Clean
clean:
	@echo "Cleaning..."
	rm -f $(OBJECTS) $(TARGET) $(BINARY) $(PROJECT).map

# Rebuild
rebuild: clean all

# Flash (requires STM32CubeProgrammer)
flash: $(BINARY)
	@echo "Flashing to STM32H562..."
	STM32_Programmer_CLI -c port=SWD -w $(BINARY) 0x08000000 -v -rst

# ==============================================================================
# STATIC ANALYSIS
# ==============================================================================

# Run static analysis (requires cppcheck)
analyze:
	@echo "Running static analysis..."
	cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES_CPP)

# ==============================================================================
# CODE METRICS
# ==============================================================================

# Count lines of code
loc:
	@echo "Lines of code:"
	@wc -l *.cpp *.h

# Show function sizes (requires nm)
functions: $(TARGET)
	@echo "Function sizes:"
	@$(PREFIX)nm --print-size --size-sort --radix=d $(TARGET) | grep " T "

# ==============================================================================
# DOCUMENTATION
# ==============================================================================

# Generate documentation (requires doxygen)
docs:
	@echo "Generating documentation..."
	doxygen Doxyfile

# ==============================================================================
# HELP
# ==============================================================================

help:
	@echo "QuickTune Build System"
	@echo "======================"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build project (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and build"
	@echo "  flash     - Flash binary to STM32H562"
	@echo "  size      - Show memory usage"
	@echo "  analyze   - Run static analysis"
	@echo "  loc       - Count lines of code"
	@echo "  functions - Show function sizes"
	@echo "  docs      - Generate documentation"
	@echo "  help      - Show this help"
	@echo ""
	@echo "Configuration:"
	@echo "  CPU:       Cortex-M33"
	@echo "  FPU:       FPv5-SP (single-precision)"
	@echo "  Optimize:  -O3 -flto"
	@echo "  Target:    STM32H562"
	@echo ""

.PHONY: all clean rebuild flash size analyze loc functions docs help
