#include "ParkingSystem.h"
#include "DataEncryption.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>

ParkingSystem::ParkingSystem(int degree) : 
    owners(degree), vehicles(degree), records(degree) {
    loadData();
}

void ParkingSystem::loadData() {
    std::ifstream ownersFile("owners.txt");
    std::string line;
    while(std::getline(ownersFile, line)) {
        line = CesarCipher::decrypt(line);
        owners.insert(Owner::fromString(line));
    }
    
    std::ifstream vehiclesFile("vehicles.txt");
    while(std::getline(vehiclesFile, line)) {
        line = CesarCipher::decrypt(line);
        vehicles.insert(Vehicle::fromString(line));
    }
    
    std::ifstream recordsFile("records.txt");
    while(std::getline(recordsFile, line)) {
        line = CesarCipher::decrypt(line);
        records.insert(ParkingRecord::fromString(line));
    }
}

void ParkingSystem::saveData() const {
    std::ofstream ownersFile("owners.txt");
    owners.traverse([&](const Owner& owner) {
        ownersFile << CesarCipher::encrypt(owner.toString()) << std::endl;
    });
    
    std::ofstream vehiclesFile("vehicles.txt");
    vehicles.traverse([&](const Vehicle& vehicle) {
        vehiclesFile << CesarCipher::encrypt(vehicle.toString()) << std::endl;
    });
    
    std::ofstream recordsFile("records.txt");
    records.traverse([&](const ParkingRecord& record) {
        recordsFile << CesarCipher::encrypt(record.toString()) << std::endl;
    });
}

void ParkingSystem::addOwner(const Owner& owner) {
    owners.insert(owner);
    saveData();
}

void ParkingSystem::removeOwner(const std::string& id) {
    Owner searchOwner;
    searchOwner.id = id;
    owners.remove(searchOwner);
    saveData();
}

Owner* ParkingSystem::findOwner(const std::string& id) {
    Owner searchOwner;
    searchOwner.id = id;
    // Note: This es una simplificación. En una implementación real,
    // necesitarías manejar la memoria de manera más segura
    if(owners.search(searchOwner)) {
        Owner* found = new Owner(searchOwner);
        return found;
    }
    return nullptr;
}

void ParkingSystem::addVehicle(const Vehicle& vehicle) {
    Owner searchOwner;
    searchOwner.id = vehicle.ownerId;
    if(!owners.search(searchOwner)) {
        throw std::runtime_error("Owner not found");
    }
    vehicles.insert(vehicle);
    saveData();
}

void ParkingSystem::removeVehicle(const std::string& plate) {
    Vehicle searchVehicle;
    searchVehicle.plate = plate;
    vehicles.remove(searchVehicle);
    saveData();
}

Vehicle* ParkingSystem::findVehicle(const std::string& plate) {
    Vehicle searchVehicle;
    searchVehicle.plate = plate;
    if(vehicles.search(searchVehicle)) {
        Vehicle* found = new Vehicle(searchVehicle);
        return found;
    }
    return nullptr;
}

void ParkingSystem::registerEntry(const std::string& plate) {
    if(!findVehicle(plate)) {
        throw std::runtime_error("Vehicle not found");
    }
    
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
    
    ParkingRecord record;
    record.plate = plate;
    record.entryTime = ss.str();
    records.insert(record);
    saveData();
}

void ParkingSystem::registerExit(const std::string& plate) {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
    
    ParkingRecord searchRecord;
    searchRecord.plate = plate;
    // Aquí deberías buscar el registro más reciente sin tiempo de salida
    // Esta es una simplificación
    if(records.search(searchRecord)) {
        searchRecord.exitTime = ss.str();
        records.remove(searchRecord);
        records.insert(searchRecord);
        saveData();
    }
}

void ParkingSystem::displayAllOwners(std::function<void(const Owner&)> display) const {
    owners.traverse(display);
}

void ParkingSystem::displayAllVehicles(std::function<void(const Vehicle&)> display) const {
    vehicles.traverse(display);
}

void ParkingSystem::displayAllRecords(std::function<void(const ParkingRecord&)> display) const {
    records.traverse(display);
}

std::vector<Vehicle> ParkingSystem::searchVehiclesByOwnerName(const std::string& ownerName) {
    std::vector<Vehicle> result;
    std::vector<std::string> ownerIds;
    
    owners.traverse([&](const Owner& owner) {
        if (owner.name.find(ownerName) != std::string::npos) {
            ownerIds.push_back(owner.id);
        }
    });
    
    vehicles.traverse([&](const Vehicle& vehicle) {
        if (std::find(ownerIds.begin(), ownerIds.end(), vehicle.ownerId) != ownerIds.end()) {
            result.push_back(vehicle);
        }
    });
    
    return result;
}

std::vector<Vehicle> ParkingSystem::searchVehiclesByAllCriteria(
    const std::string& brand,
    const std::string& model,
    const std::string& color
) {
    std::vector<Vehicle> result;
    
    vehicles.traverse([&](const Vehicle& vehicle) {
        bool matches = true;
        if (!brand.empty() && vehicle.brand.find(brand) == std::string::npos) matches = false;
        if (!model.empty() && vehicle.model.find(model) == std::string::npos) matches = false;
        if (!color.empty() && vehicle.color.find(color) == std::string::npos) matches = false;
        if (matches) result.push_back(vehicle);
    });
    
    return result;
}

bool ParkingSystem::isTimeInRange(
    const std::string& timeToCheck,
    const std::string& startTime,
    const std::string& endTime
) {
    return timeToCheck >= startTime && timeToCheck <= endTime;
}

std::vector<ParkingRecord> ParkingSystem::searchRecordsByTimeRange(
    const std::string& startTime,
    const std::string& endTime
) {
    std::vector<ParkingRecord> result;
    
    records.traverse([&](const ParkingRecord& record) {
        if (isTimeInRange(record.entryTime, startTime, endTime)) {
            result.push_back(record);
        }
    });
    
    return result;
}

std::vector<Vehicle> ParkingSystem::searchVehiclesByLocation(bool nearExit) {
    std::vector<Vehicle> result;
    std::vector<std::string> recentPlates;
    
    // Obtener los últimos registros (más cercanos a la salida o entrada)
    records.traverse([&](const ParkingRecord& record) {
        if (nearExit && record.exitTime.empty()) {
            recentPlates.push_back(record.plate);
        }
    });
    
    // Obtener los vehículos correspondientes
    vehicles.traverse([&](const Vehicle& vehicle) {
        if (std::find(recentPlates.begin(), recentPlates.end(), vehicle.plate) != recentPlates.end()) {
            result.push_back(vehicle);
        }
    });
    
    return result;
}

std::vector<ParkingRecord> ParkingSystem::searchRecordsByPlateAndDate(
    const std::string& plate,
    const std::string& date
) {
    std::vector<ParkingRecord> result;
    
    records.traverse([&](const ParkingRecord& record) {
        if (record.plate == plate && record.entryTime.find(date) != std::string::npos) {
            result.push_back(record);
        }
    });
    
    return result;
}