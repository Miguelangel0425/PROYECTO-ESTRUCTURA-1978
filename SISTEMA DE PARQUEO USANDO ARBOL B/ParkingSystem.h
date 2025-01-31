#pragma once
#include "BTree.h"
#include "Models.h"
#include <string>
#include <functional>
#include "ParkingVisualization.h"

class ParkingSystem {
private:
    BTree<Owner> owners;
    BTree<Vehicle> vehicles;
    BTree<ParkingRecord> records;
    ParkingVisualization parkingLayout;
    
    void loadData();
    void saveData() const;
    Owner* findOwnerInTree(const std::string& id) const;
    void reloadData();
public:
    ParkingSystem(int degree = 3);
    
    //grafica
    void displayParkingLayout() const;
    void getVehicleLocation(const std::string& plate);

    // Operaciones de propietarios
    void addOwner(const Owner& owner);
    void removeOwner(const std::string& id);
    Owner* findOwner(const std::string& id);
    void updateOwner(const Owner& owner);
    bool deleteOwner(const std::string& id);
    bool isOwnerRegistered(const std::string& id);

    // Operaciones de vehículos
    void addVehicle(const Vehicle& vehicle);
    void removeVehicle(const std::string& plate);
    Vehicle* findVehicle(const std::string& plate);
    bool deleteVehicle(const std::string& plate);
    std::vector<Vehicle> getVehiclesByOwnerId(const std::string& ownerId);
    bool isPlateRegistered(const std::string& plate);
    bool updateVehicle(const Vehicle& vehicle);
    int getVehicleCount() const;

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
