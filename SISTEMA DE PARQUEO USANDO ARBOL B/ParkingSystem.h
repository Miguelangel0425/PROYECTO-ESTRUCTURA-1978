#pragma once
#include "BTree.h"
#include "Models.h"
#include <string>
#include <functional>

class ParkingSystem {
private:
    BTree<Owner> owners;
    BTree<Vehicle> vehicles;
    BTree<ParkingRecord> records;
    
    void loadData();
    void saveData() const;
    
public:
    ParkingSystem(int degree = 3);
    
    // Operaciones de propietarios
    void addOwner(const Owner& owner);
    void removeOwner(const std::string& id);
    Owner* findOwner(const std::string& id);
    
    // Operaciones de vehículos
    void addVehicle(const Vehicle& vehicle);
    void removeVehicle(const std::string& plate);
    Vehicle* findVehicle(const std::string& plate);
    
    // Operaciones de registro
    void registerEntry(const std::string& plate);
    void registerExit(const std::string& plate);
    
    // Utilidades
    void displayAllOwners(std::function<void(const Owner&)> display) const;
    void displayAllVehicles(std::function<void(const Vehicle&)> display) const;
    void displayAllRecords(std::function<void(const ParkingRecord&)> display) const;

    // Búsquedas avanzadas
    std::vector<Vehicle> searchVehiclesByOwnerName(const std::string& ownerName);
    std::vector<Vehicle> searchVehiclesByAllCriteria(
        const std::string& brand = "", 
        const std::string& model = "", 
        const std::string& color = ""
    );
    std::vector<ParkingRecord> searchRecordsByTimeRange(
        const std::string& startTime,
        const std::string& endTime
    );
    std::vector<Vehicle> searchVehiclesByLocation(bool nearExit);
    std::vector<ParkingRecord> searchRecordsByPlateAndDate(
        const std::string& plate,
        const std::string& date
    );
    
private:
    // Helper method for time comparison
    bool isTimeInRange(
        const std::string& timeToCheck,
        const std::string& startTime,
        const std::string& endTime
    );
};
