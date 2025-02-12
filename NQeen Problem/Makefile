# Definición de variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
OBJ_DIR = obj
SRCS = main.cpp NQueens.cpp
OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)
EXEC = nqueens

# Reglas
.PHONY: all clean cleanall run directories

# Regla principal que depende de directories y el ejecutable
all: directories $(EXEC)

# Crear directorio para objetos
directories:
	mkdir -p $(OBJ_DIR)

# Regla para el ejecutable
$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(SFML_LIBS)

# Regla para los objetos
$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos objeto
clean:
	rm -rf $(OBJ_DIR)

# Limpiar todo lo generado por el programa
cleanall: clean
	rm -f $(EXEC)
	rm -f queens_steps.txt
	rm -f *.log
	rm -f core.*

# Regla para ejecutar el programa
run: all
	./$(EXEC)
