#pragma once
#include <vector>
#include <string>
#include <map>

class ParkingVisualization {

    private:
    static const int ROWS = 5;
    static const int COLS = 10;
    std::map<std::string, std::pair<int, int>> parkingMap; // Mapa de placa a posición
    std::vector<std::vector<std::string>> parkingGrid;
    
public:
    ParkingVisualization();
    bool parkVehicle(const std::string& plate);
    bool removeVehicle(const std::string& plate);
    void display() const;
    bool isOccupied(int row, int col) const;
    std::pair<int, int> getVehiclePosition(const std::string& plate) const;
    bool loadState(const std::string& filename);
    bool saveState(const std::string& filename) const;
};