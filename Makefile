# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g -O0 #-static -static-libgcc -static-libstdc++ -fPIC #-Wl,-subsystem,windows

# Directories
SRC_DIR = src
VENDOR_DIR = $(SRC_DIR)/vendor
OBJ_DIR = obj
INCLUDE_DIR = ./include  # Directory for header files
BUILD_DIR = bin
LIB_DIR = ./lib  # Directory for static libraries

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(VENDOR_DIR)/**/*.c)

# Include paths
INCLUDE = -I$(INCLUDE_DIR)

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(filter %.cpp, $(SRCS))) \
       $(patsubst $(VENDOR_DIR)/%.c, $(OBJ_DIR)/%.o, $(filter %.c, $(SRCS)))

# Libraries to link
LIBS = -L$(LIB_DIR) -lglfw3 -l:libsoil2-debug.lib -lgdi32 -lwinmm -lopengl32 

# Executable name
TARGET = $(BUILD_DIR)/main

# Dependency files
DEPS = $(OBJS:.o=.d)

# Rule to build the target
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ $^ $(LIBS)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -c $< -o $@

# Rule to build object files from vendor files
$(OBJ_DIR)/%.o: $(VENDOR_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -MMD -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean up build files
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	rm -f $(OBJ_DIR)/*.d $(TARGET)

# Phony targets
.PHONY: clean
