# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Directories
OBJ_DIR = obj
BIN_DIR = .
DATA_DIR = data

# Project files
SOURCES = main.cpp \
          ParkingSystem.cpp \
          DataEncryption.cpp \
          InteractiveMenu.cpp \
          validaciones.cpp \
		  ParkingVisualization.cpp\
          Models.cpp

# Object files
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# Executable
EXECUTABLE = $(BIN_DIR)/parking_system

# Data files
DATA_FILES = owners.txt vehicles.txt records.txt

# Main target
all: init $(EXECUTABLE)

# Initialize directories and data files
init: make_dirs make_data_files

make_dirs:
	@mkdir -p $(OBJ_DIR)

make_data_files:
	@for file in $(DATA_FILES); do \
		if [ ! -f $$file ]; then \
			touch $$file; \
			echo "Created $$file"; \
		fi \
	done

# Linking
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)

# Compilation of source files to object files
$(OBJ_DIR)/%.o: %.cpp
	@echo "Compiling $< into $@"  # Esto mostrará qué archivo se está compilando
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXECUTABLE)

# Deep clean (includes data files)
cleanall: clean
	rm -f $(DATA_FILES)

# Run
run: all
	./$(EXECUTABLE)

.PHONY: all clean cleanall run init make_dirs make_data_files
