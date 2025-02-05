#pragma once
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "BTree.h" 
#include "DataEncryption.h" 
#include "Models.h"
#include "SpaceNode.h"
#include <iostream>
#include <string>




class ParkingVisualization {
private:
    std::map<std::string, std::pair<int, int>> parkingMap; // Mapa de placa a posición
    std::vector<std::vector<std::string>> parkingGrid;
    BTree<SpaceNode> spaceTree;
    

    // Posiciones de entradas y salidas
    const std::vector<std::pair<int, int>> ENTRIES = {{0, 0}, {0, COLS - 1}};
    const std::vector<std::pair<int, int>> EXITS = {{ROWS - 1, 0}, {ROWS - 1, COLS - 1}};

    void initializeSpaces();
    Vehicle* findVehicle(const std::string& plate) const;  // Declaración de findVehicle

public:
    const BTree<SpaceNode>& getSpaceTree() const { return spaceTree; }  // Árbol B para espacios ocupados
    static const int ROWS = 10;
    static const int COLS = 10;
    static int getRows() { return ROWS; }
    static int getCols() { return COLS; }

    ParkingVisualization();
    bool parkVehicle(const std::string& plate);
    bool removeVehicle(const std::string& plate);
    void display() const;
    bool isOccupied(int row, int col) const;
    std::pair<int, int> getVehiclePosition(const std::string& plate) const;
    bool loadState(const std::string& filename);
    bool saveState(const std::string& filename) const;
    void displaySpaceTree() const;
    std::vector<std::pair<std::string, double>> getVehiclesByDistance(bool nearExit) const; // Cambio a string en lugar de Vehicle
};