#include "ParkingSystem.h"
#include "DataEncryption.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <memory>

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

void ParkingSystem::reloadData() {
    // Limpiar árboles existentes
    owners = BTree<Owner>(3);
    vehicles = BTree<Vehicle>(3);
    records = BTree<ParkingRecord>(3);
    loadData();
}

void ParkingSystem::saveData() const {
    // Guardar propietarios
    {
        std::ofstream ownersFile("owners.txt", std::ios::trunc);
        if (!ownersFile) {
            throw std::runtime_error("No se pudo abrir el archivo de propietarios");
        }
        owners.traverse([&](const Owner& owner) {
            ownersFile << CesarCipher::encrypt(owner.toString()) << std::endl;
        });
    }
    
    // Guardar vehículos
    {
        std::ofstream vehiclesFile("vehicles.txt", std::ios::trunc);
        if (!vehiclesFile) {
            throw std::runtime_error("No se pudo abrir el archivo de vehículos");
        }
        vehicles.traverse([&](const Vehicle& vehicle) {
            vehiclesFile << CesarCipher::encrypt(vehicle.toString()) << std::endl;
        });
    }
    
    // Guardar registros
    {
        std::ofstream recordsFile("records.txt", std::ios::trunc);
        if (!recordsFile) {
            throw std::runtime_error("No se pudo abrir el archivo de registros");
        }
        records.traverse([&](const ParkingRecord& record) {
            recordsFile << CesarCipher::encrypt(record.toString()) << std::endl;
        });
    }
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
    return findOwnerInTree(id);
}

Owner* ParkingSystem::findOwnerInTree(const std::string& id) const {
    std::unique_ptr<Owner> result;
    owners.traverse([&](const Owner& owner) {
        if (owner.id == id) {
            result = std::make_unique<Owner>(owner);
        }
    });
    return result ? new Owner(*result) : nullptr;
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

void ParkingSystem::updateOwner(const Owner& owner) {
    Owner* existing = findOwnerInTree(owner.id);
    if (!existing) {
        throw std::runtime_error("Propietario no encontrado");
    }
    delete existing;

    // Eliminar el propietario existente
    Owner searchOwner;
    searchOwner.id = owner.id;
    owners.remove(searchOwner);

    // Insertar el nuevo propietario
    owners.insert(owner);

    // Guardar y recargar para asegurar consistencia
    saveData();
    reloadData();
}

bool ParkingSystem::deleteOwner(const std::string& id) {
    Owner* existing = findOwnerInTree(id);
    if (!existing) {
        return false;
    }
    delete existing;

    // Eliminar vehículos del propietario
    auto ownerVehicles = getVehiclesByOwnerId(id);
    for (const auto& vehicle : ownerVehicles) {
        deleteVehicle(vehicle.plate);
    }

    // Eliminar el propietario
    Owner searchOwner;
    searchOwner.id = id;
    owners.remove(searchOwner);

    // Guardar y recargar para asegurar consistencia
    saveData();
    reloadData();
    return true;
}

bool ParkingSystem::deleteVehicle(const std::string& plate){
    Vehicle* existing = findVehicle(plate);
    if (!existing) {
        return false;
    }
    delete existing;

    // Eliminar el vehículo
    Vehicle searchVehicle;
    searchVehicle.plate = plate;
    vehicles.remove(searchVehicle);

    // Guardar y recargar para asegurar consistencia
    saveData();
    reloadData();
    return true;

}

std::vector<Vehicle> ParkingSystem::getVehiclesByOwnerId(const std::string& ownerId){
    std::vector<Vehicle> result;
    vehicles.traverse([&](const Vehicle& vehicle){
        if(vehicle.ownerId == ownerId){
            result.push_back(vehicle);
        }
    });
    return result; 
}

void ParkingSystem::registerEntry(const std::string& plate) {
    if(!findVehicle(plate)) {
        throw std::runtime_error("Vehicle not found");
    }
    
    if (!parkingLayout.parkVehicle(plate)) {
        throw std::runtime_error("No parking spaces available");
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
    parkingLayout.removeVehicle(plate);
    
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
    
    ParkingRecord searchRecord;
    searchRecord.plate = plate;
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

void ParkingSystem::displayParkingLayout() const {
    parkingLayout.display();
}

void ParkingSystem::getVehicleLocation(const std::string& plate) {
    auto pos = parkingLayout.getVehiclePosition(plate);
    if (pos.first != -1) {
        std::cout << "Vehicle " << plate << " is parked at position (" 
                 << pos.first + 1 << ", " << pos.second + 1 << ")\n";
    } else {
        std::cout << "Vehicle " << plate << " is not currently parked.\n";
    }
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