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
            std::string line = owner.toString();
            std::string encrypted = CesarCipher::encrypt(line);
            ownersFile << encrypted << std::endl;
            ownersFile.flush(); // Forzar la escritura
        });
    }
    
    // Similar para vehicles.txt y records.txt

    {
        std::ofstream vehiclesFile("vehicles.txt", std::ios::trunc);
        if(!vehiclesFile){
            throw std::runtime_error("No se pudo abrir el archivo de vahiculos.");
        }
        vehicles.traverse([&](const Vehicle& vehicle){
            std::string line = vehicle.toString();
            std::string encrypted = CesarCipher::encrypt(line);
            vehiclesFile << encrypted << std::endl;
            vehiclesFile.flush();
        });
    }

    {
        std::ofstream recordsFile("records.txt", std::ios::trunc);
        if(!recordsFile){
            throw std::runtime_error("No se pudo abrir el archivo de registros.");
        }
        records.traverse([&](const ParkingRecord& record){
            std::string line = record.toString();
            std::string encrypted = CesarCipher::encrypt(line);
            recordsFile << encrypted << std::endl;
            recordsFile.flush();
        });
    }
}

bool ParkingSystem::isPlateRegistered(const std::string& plate) {
    Vehicle searchVehicle;
    searchVehicle.plate = plate;
    bool found = false;
    vehicles.traverse([&](const Vehicle& v) {
        if (v.plate == plate) {
            found = true;
        }
    });
    return found;
}

bool ParkingSystem::isOwnerRegistered(const std::string& id) {
    Owner searchOwner;
    searchOwner.id = id;
    bool found = false;
    owners.traverse([&](const Owner& o) {
        if (o.id == id) {
            found = true;
        }
    });
    return found;
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
    Vehicle* result = nullptr;
    vehicles.traverse([&](const Vehicle& vehicle) {
        if (vehicle.plate == plate) {
            result = new Vehicle(vehicle);  // Copia completa del vehículo encontrado
        }
    });
    return result;
}


void ParkingSystem::updateOwner(const Owner& owner) {
    // Primero verificar si el propietario existe
    Owner* existing = findOwnerInTree(owner.id);
    if (!existing) {
        throw std::runtime_error("Propietario no encontrado");
    }
    delete existing;

    // Crear una copia temporal de los árboles
    BTree<Owner> tempOwners(3);
    
    // Copiar todos los propietarios excepto el que se va a actualizar
    owners.traverse([&](const Owner& o) {
        if (o.id != owner.id) {
            tempOwners.insert(o);
        }
    });
    
    // Insertar el propietario actualizado
    tempOwners.insert(owner);
    
    // Reemplazar el árbol original
    owners = std::move(tempOwners);
    
    // Guardar los cambios
    saveData();
}

bool ParkingSystem::updateVehicle(const Vehicle& vehicle) {
    // Verificar si el vehículo existe
    Vehicle* existing = findVehicle(vehicle.plate);
    if (!existing) {
        return false;
    }
    delete existing;

    // Crear una copia temporal del árbol
    BTree<Vehicle> tempVehicles(3);
    
    // Copiar todos los vehículos excepto el que se va a actualizar
    vehicles.traverse([&](const Vehicle& v) {
        if (v.plate != vehicle.plate) {
            tempVehicles.insert(v);
        }
    });
    
    // Insertar el vehículo actualizado
    tempVehicles.insert(vehicle);
    
    // Reemplazar el árbol original
    vehicles = std::move(tempVehicles);
    
    // Guardar los cambios
    saveData();
    return true;
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
        throw std::runtime_error("Vehiculo No Encontrado");
    }
    
    if (!parkingLayout.parkVehicle(plate)) {
        throw std::runtime_error("Sin Espacio en el Parqueadero");
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
    
    // Crear un árbol temporal para los registros actualizados
    BTree<ParkingRecord> tempRecords(3);
    
    // Copiar todos los registros, actualizando el que corresponde
    records.traverse([&](const ParkingRecord& record) {
        if (record.plate == plate && record.exitTime.empty()) {
            // Crear registro actualizado
            ParkingRecord updatedRecord = record;
            updatedRecord.exitTime = ss.str();
            tempRecords.insert(updatedRecord);
        } else {
            // Copiar el resto de registros sin modificar
            tempRecords.insert(record);
        }
    });
    
    // Reemplazar el árbol original con el actualizado
    records = std::move(tempRecords);
    saveData();
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

int ParkingSystem::getVehicleCount() const {
    int count = 0;
    vehicles.traverse([&](const Vehicle&) {
        count++;
    });
    return count;
}