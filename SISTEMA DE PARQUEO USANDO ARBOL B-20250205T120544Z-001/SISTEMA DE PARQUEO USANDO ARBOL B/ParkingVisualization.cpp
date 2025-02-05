#include "ParkingVisualization.h"
#include "BTree.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <random>
#include <cmath>  // Para sqrt y pow

ParkingVisualization::ParkingVisualization() : spaceTree(3) {
    parkingGrid = std::vector<std::vector<std::string>>(ROWS, std::vector<std::string>(COLS, "EMPTY"));
    initializeSpaces();
    loadState("parking_layout.txt");
}

void ParkingVisualization::initializeSpaces() {
    // Implementación opcional si necesitas inicializar algo adicional
}

bool ParkingVisualization::parkVehicle(const std::string& plate) {
    if (parkingMap.find(plate) != parkingMap.end()) {
        return true;
    }

    std::vector<std::pair<int, int>> availableSpots;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (parkingGrid[i][j] == "EMPTY") {
                availableSpots.push_back({i, j});
            }
        }
    }

    if (availableSpots.empty()) {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, availableSpots.size() - 1);

    int randomIndex = dis(gen);
    int row = availableSpots[randomIndex].first;
    int col = availableSpots[randomIndex].second;

    int spaceNumber = row * COLS + col + 1;  // Número de espacio (1-100)

    parkingGrid[row][col] = plate;
    parkingMap[plate] = {row, col};

    // Agregar al árbol B
    SpaceNode node(spaceNumber, plate);
    spaceTree.insert(node);

    std::cout << "\033[1;32mVehículo " << plate << " estacionado en el espacio #"
              << spaceNumber << "\033[0m\n";

    saveState("parking_layout.txt");
    return true;
}

bool ParkingVisualization::removeVehicle(const std::string& plate) {
    auto it = parkingMap.find(plate);
    if (it != parkingMap.end()) {
        int row = it->second.first;
        int col = it->second.second;
        int spaceNumber = row * COLS + col + 1;  // Calcular el número de espacio
        
        // Crear un nodo temporal para buscar y eliminar del árbol B
        SpaceNode nodeToRemove(spaceNumber, plate);
        spaceTree.remove(nodeToRemove);
        
        // Actualizar el grid y el mapa
        parkingGrid[row][col] = "EMPTY";
        parkingMap.erase(it);
        
        saveState("parking_layout.txt");
        return true;
    }
    return false;
}

void ParkingVisualization::display() const {
    std::cout << "\033[1;36m";
    std::cout << "========================= PARKING LAYOUT =========================\n";
    std::cout << "E = Entrada | S = Salida | [] = Vacío | ## = Ocupado\n\n";

    for (int i = 0; i < ROWS; i++) {
        std::cout << "| ";
        for (int j = 0; j < COLS; j++) {
            bool isEntry = std::any_of(ENTRIES.begin(), ENTRIES.end(), [&](const auto& p) {
                return p.first == i && p.second == j;
            });
            bool isExit = std::any_of(EXITS.begin(), EXITS.end(), [&](const auto& p) {
                return p.first == i && p.second == j;
            });

            if (isEntry) {
                std::cout << "\033[1;32m[E] \033[1;36m";
            } else if (isExit) {
                std::cout << "\033[1;31m[S] \033[1;36m";
            } else {
                int spaceNum = i * COLS + j + 1;
                if (parkingGrid[i][j] == "EMPTY") {
                    std::cout << "\033[1;37m[" << std::setw(2) << spaceNum << "]\033[1;36m ";
                } else {
                    std::cout << "\033[1;33m" << std::setw(4) << parkingGrid[i][j].substr(0, 3) << "\033[1;36m ";
                }
            }
        }
        std::cout << "|\n";
    }
    std::cout << "==============================================================\n";
    std::cout << "\033[0m";
}

void ParkingVisualization::displaySpaceTree() const {
    std::cout << "\n=== Arbol B de Espacios Ocupados ===\n";
    spaceTree.display();  // Llama al método display() de `BTree`
    std::cout << "===================================\n";
}


std::pair<int, int> ParkingVisualization::getVehiclePosition(const std::string& plate) const {
    auto it = parkingMap.find(plate);
    if (it != parkingMap.end()) {
        return it->second;  // Retorna la posición (fila, columna)
    }
    return {-1, -1};  // Retorna posición inválida si no se encuentra
}

bool ParkingVisualization::loadState(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error al abrir el archivo de estado: " << filename << std::endl;
        return false;
    }

    parkingMap.clear();
    spaceTree = BTree<SpaceNode>(3); // Limpiar el árbol antes de cargar nuevos datos

    std::string line;
    bool readingTree = false;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            file >> parkingGrid[i][j];
            if (parkingGrid[i][j] != "EMPTY" && parkingGrid[i][j] != "---TREE---") {
                parkingMap[parkingGrid[i][j]] = {i, j};
            }
        }
    }

    while (std::getline(file, line)) {
        if (line == "---TREE---") {
            readingTree = true;
            continue;
        }

        if (readingTree) {
            int spaceNumber;
            std::string plate;
            std::istringstream iss(line);
            if (iss >> spaceNumber >> plate) {
                spaceTree.insert(SpaceNode(spaceNumber, plate));
            }
        }
    }

    file.close();
    return true;
}


bool ParkingVisualization::saveState(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error al guardar el archivo de estado: " << filename << std::endl;
        return false;
    }

    // Guardar la representación visual de la grilla
    for (const auto& row : parkingGrid) {
        for (const auto& space : row) {
            file << space << " ";
        }
        file << "\n";
    }

    // Separador para distinguir la grilla del árbol B
    file << "---TREE---\n";

    // Guardar el árbol B con los espacios ocupados
    spaceTree.traverse([&](const SpaceNode& node) {
        file << node.spaceNumber << " " << node.plate << "\n";
    });

    file.close();
    return true;
}
