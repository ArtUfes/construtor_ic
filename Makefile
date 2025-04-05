# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -I src

# Paths
TOY = instances/toy_UFES.ctt
ALG2401 = instances/ALG_20241.txt
ALG2302 = instances/ALG_20232.txt
VIX2401 = instances/VIX_20241.txt
VIX2302 = instances/VIX_20232.txt


# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INPUT = $(ALG2302)

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp) main.cpp
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

# Executable name
EXEC = main

# Default target
all: $(BIN_DIR)/$(EXEC)


# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rule to build the executable
$(BIN_DIR)/$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean

run: $(BIN_DIR)/$(EXEC)
	make
	clear
	./$(BIN_DIR)/$(EXEC) $(ARGS) $(INPUT)

val:
	make
	clear
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BIN_DIR)/$(EXEC) $(ARGS) $(INPUT)