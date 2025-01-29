// ParkingVisualization.cpp
#include "ParkingVisualization.h"
#include "DataEncryption.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

ParkingVisualization::ParkingVisualization() {
    // Inicializar grid vacío
    parkingGrid = std::vector<std::vector<std::string>>(ROWS, std::vector<std::string>(COLS, "EMPTY"));
    loadState("parking_layout.txt");
}

bool ParkingVisualization::parkVehicle(const std::string& plate) {
    // Si el vehículo ya está estacionado, mantener su posición
    if (parkingMap.find(plate) != parkingMap.end()) {
        return true;
    }
    
    // Buscar primer espacio disponible
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (parkingGrid[i][j] == "EMPTY") {
                parkingGrid[i][j] = plate;
                parkingMap[plate] = {i, j};
                saveState("parking_layout.txt");
                return true;
            }
        }
    }
    return false;
}

bool ParkingVisualization::removeVehicle(const std::string& plate) {
    auto it = parkingMap.find(plate);
    if (it != parkingMap.end()) {
        int row = it->second.first;
        int col = it->second.second;
        parkingGrid[row][col] = "EMPTY";
        parkingMap.erase(it);
        saveState("parking_layout.txt");
        return true;
    }
    return false;
}

void ParkingVisualization::display() const {
    std::cout << "\033[1;36m";  // Color cian brillante
    std::cout << "===================== PARKING LAYOUT=====================\n";
    
    for (int i = 0; i < ROWS; i++) {
        std::cout << "| ";
        for (int j = 0; j < COLS; j++) {
            if (parkingGrid[i][j] == "EMPTY") {
                std::cout << "\033[1;32m"; // Verde para espacios vacíos
                std::cout << std::setw(8) << "[]";
            } else {
                std::cout << "\033[1;31m"; // Rojo para espacios ocupados
                std::cout << std::setw(8) << parkingGrid[i][j].substr(0, 7);
            }
            std::cout << "\033[1;36m"; // Volver al color del marco
            std::cout << " ";
        }
        std::cout << "|\n";
    }
    
    std::cout << "=========================================================\n";
    std::cout << "\033[0m";  // Resetear color
}

bool ParkingVisualization::isOccupied(int row, int col) const {
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        return parkingGrid[row][col] != "EMPTY";
    }
    return false;
}

std::pair<int, int> ParkingVisualization::getVehiclePosition(const std::string& plate) const {
    auto it = parkingMap.find(plate);
    if (it != parkingMap.end()) {
        return it->second;
    }
    return {-1, -1};
}

bool ParkingVisualization::saveState(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) return false;
    
    for (const auto& pair : parkingMap) {
        std::string line = pair.first + "," + 
                          std::to_string(pair.second.first) + "," + 
                          std::to_string(pair.second.second);
        file << CesarCipher::encrypt(line) << std::endl;
    }
    return true;
}

bool ParkingVisualization::loadState(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;
    
    // Limpiar estado actual
    parkingGrid = std::vector<std::vector<std::string>>(ROWS, std::vector<std::string>(COLS, "EMPTY"));
    parkingMap.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        line = CesarCipher::decrypt(line);
        std::stringstream ss(line);
        std::string plate;
        std::string row_str, col_str;
        
        std::getline(ss, plate, ',');
        std::getline(ss, row_str, ',');
        std::getline(ss, col_str, ',');
        
        int row = std::stoi(row_str);
        int col = std::stoi(col_str);
        
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            parkingGrid[row][col] = plate;
            parkingMap[plate] = {row, col};
        }
    }
    return true;
}