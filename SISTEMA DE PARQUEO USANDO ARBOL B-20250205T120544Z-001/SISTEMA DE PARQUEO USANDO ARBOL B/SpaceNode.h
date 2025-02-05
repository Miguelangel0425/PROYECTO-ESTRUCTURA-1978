#pragma once
#include <string>


class SpaceNode {
public:
    int spaceNumber;
    std::string plate;

    SpaceNode() : spaceNumber(0), plate("") {} // Constructor vacío necesario para BTree

    SpaceNode(int number, std::string p) : spaceNumber(number), plate(std::move(p)) {}

    bool operator<(const SpaceNode& other) const {
        return this->spaceNumber < other.spaceNumber;
    }

    bool operator>(const SpaceNode& other) const {
        return this->spaceNumber > other.spaceNumber;
    }

    bool operator==(const SpaceNode& other) const {
        return this->spaceNumber == other.spaceNumber;
    }

    std::string toString() const {
        return std::to_string(spaceNumber) + ":" + plate;
    }

    // ✅ Sobrecarga del operador `<<` para imprimir `SpaceNode`
    friend std::ostream& operator<<(std::ostream& os, const SpaceNode& node) {
        os << "Space " << node.spaceNumber << " - Plate: " << node.plate;
        return os;
    }
};