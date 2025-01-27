#pragma once
#include <string>
#include <chrono>
#include "Validaciones.h"

struct Owner {
    std::string id;
    std::string name;
    std::string phone;
    std::string email;
    
    std::string toString() const;
    static Owner fromString(const std::string& str);
    bool operator<(const Owner& other) const { return id < other.id; }
    bool operator>(const Owner& other) const { return id > other.id; }
    bool operator==(const Owner& other) const { return id == other.id; }
    static bool isValid(const Owner& owner) {
        return Validaciones::validarCedula(owner.id) &&
               Validaciones::validarCorreo(owner.email);
    }
};

struct Vehicle {
    std::string plate;
    std::string brand;
    std::string model;
    std::string color;
    std::string ownerId;
    
    std::string toString() const;
    static Vehicle fromString(const std::string& str);
    bool operator<(const Vehicle& other) const { return plate < other.plate; }
    bool operator>(const Vehicle& other) const { return plate > other.plate; }
    bool operator==(const Vehicle& other) const { return plate == other.plate; }
};

struct ParkingRecord {
    std::string plate;
    std::string entryTime;
    std::string exitTime;
    
    std::string toString() const;
    static ParkingRecord fromString(const std::string& str);
    bool operator<(const ParkingRecord& other) const { return plate < other.plate; }
    bool operator>(const ParkingRecord& other) const { return plate > other.plate; }
    bool operator==(const ParkingRecord& other) const { return plate == other.plate; }
};
