#include "Models.h"
#include <sstream>

std::string Owner::toString() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << phone << "|" << email;
    return ss.str();
}

Owner Owner::fromString(const std::string& str) {
    std::stringstream ss(str);
    std::string token;
    Owner owner;
    
    std::getline(ss, owner.id, '|');
    std::getline(ss, owner.name, '|');
    std::getline(ss, owner.phone, '|');
    std::getline(ss, owner.email, '|');
    
    return owner;
}

std::string Vehicle::toString() const {
    std::stringstream ss;
    ss << plate << "|" << brand << "|" << model << "|" << color << "|" << ownerId;
    return ss.str();
}

Vehicle Vehicle::fromString(const std::string& str) {
    std::stringstream ss(str);
    std::string token;
    Vehicle vehicle;
    
    std::getline(ss, vehicle.plate, '|');
    std::getline(ss, vehicle.brand, '|');
    std::getline(ss, vehicle.model, '|');
    std::getline(ss, vehicle.color, '|');
    std::getline(ss, vehicle.ownerId, '|');
    
    return vehicle;
}

std::string ParkingRecord::toString() const {
    std::stringstream ss;
    ss << plate << "|" << entryTime << "|" << exitTime;
    return ss.str();
}

ParkingRecord ParkingRecord::fromString(const std::string& str) {
    std::stringstream ss(str);
    std::string token;
    ParkingRecord record;
    
    std::getline(ss, record.plate, '|');
    std::getline(ss, record.entryTime, '|');
    std::getline(ss, record.exitTime, '|');
    
    return record;
}
