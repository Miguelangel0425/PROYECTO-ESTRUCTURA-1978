#include "ParkingSystem.h"
#include <iostream>
#include <conio.h>
#include "InteractiveMenu.h"
#include "Validaciones.h"


// Add missing display functions
void displayVehicle(const Vehicle& vehicle) {
    std::cout << "Plate: " << vehicle.plate << "\n"
              << "Brand: " << vehicle.brand << "\n"
              << "Model: " << vehicle.model << "\n"
              << "Color: " << vehicle.color << "\n"
              << "Owner ID: " << vehicle.ownerId << "\n\n";
}

void displayRecord(const ParkingRecord& record) {
    std::cout << "Plate: " << record.plate << "\n"
              << "Entry Time: " << record.entryTime << "\n"
              << "Exit Time: " << record.exitTime << "\n\n";
}

int main() {
    ParkingSystem system(3);
    
    std::vector<std::string> mainMenuOptions = {
        "\033[1;33mAdd Owner\033[0m",            
        "\033[1;33mAdd Vehicle\033[0m",         
        "\033[1;33mRegister Entry\033[0m",      
        "\033[1;33mRegister Exit\033[0m",       
        "\033[1;33mDisplay All Owners\033[0m", 
        "\033[1;33mDisplay All Vehicles\033[0m",
        "\033[1;33mDisplay All Records\033[0m", 
        "\033[1;33mAdvanced Search\033[0m",     
        "\033[1;33mExit\033[0m"                 
    };
    
    InteractiveMenu mainMenu(mainMenuOptions,"Parking System");
    
    while (true) {
        std::system("cls");
        int choice = mainMenu.show();
        
        try {
            switch (choice) {
                case 0: {
                    Owner owner;
                    owner.id = Validaciones::ingresarCedula("Ingrese el número de cédula: ");
                    owner.name = Validaciones::ingresarString("Ingrese el nombre: ");
                    owner.phone = Validaciones::ingresarTelefono("Ingrese el teléfono: "); // Changed from ingresarTelefono which doesn't exist
                    owner.email = Validaciones::ingresarCorreo("Ingrese el correo: ");
    
                    if (Owner::isValid(owner)) {
                        system.addOwner(owner);
                        std::cout << "Propietario agregado exitosamente." << std::endl;
                    } else {
                        throw std::runtime_error("Datos de propietario inválidos");
                    }
                    break;
                }
                case 1: {
                    Vehicle vehicle;
                    std::cout << "Enter plate number: ";
                    std::getline(std::cin, vehicle.plate);
                    std::cout << "Enter brand: ";
                    std::getline(std::cin, vehicle.brand);
                    std::cout << "Enter model: ";
                    std::getline(std::cin, vehicle.model);
                    std::cout << "Enter color: ";
                    std::getline(std::cin, vehicle.color);
                    
                    while (true) {
                        std::cout << "Enter owner ID: ";
                        std::getline(std::cin, vehicle.ownerId);
                        
                        if (system.findOwner(vehicle.ownerId)) {
                            system.addVehicle(vehicle);
                            break;
                        } else {
                            std::vector<std::string> idErrorOptions = {
                                "\033[1;33mRe-enter Owner ID\033[0m",
                                "\033[1;33mAdd New Owner\033[0m",
                                "\033[1;33mCancel\033[0m"
                            };
                            int idErrorChoice = InteractiveMenu::showSubMenu(idErrorOptions, "Invalid Owner ID");
                            
                            if (idErrorChoice == 0) {
                                continue;
                            } else if (idErrorChoice == 1) {
                                Owner owner;
                                owner.id = Validaciones::ingresarCedula("Ingrese el número de cédula: ");
                                owner.name = Validaciones::ingresarString("Ingrese el nombre: ");
                                owner.phone = Validaciones::ingresarTelefono("Ingrese el teléfono: ");
                                owner.email = Validaciones::ingresarCorreo("Ingrese el correo: ");
                                
                                if (Owner::isValid(owner)) {
                                    system.addOwner(owner);
                                    vehicle.ownerId = owner.id;
                                    system.addVehicle(vehicle);
                                    break;
                                } else {
                                    std::cout << "Datos de propietario inválidos. Cancelando operación." << std::endl;
                                    break;
                                }
                            } else {
                                std::cout << "Operacion cancelada." << std::endl;
                                break;
                            }
                        }
                    }
                    break;
                }
                case 2: {
                    std::string plate;
                    std::cout << "Enter plate number: ";
                    std::getline(std::cin, plate);
                    system.registerEntry(plate);
                    break;
                }
                case 3: {
                    std::string plate;
                    std::cout << "Enter plate number: ";
                    std::getline(std::cin, plate);
                    system.registerExit(plate);
                    break;
                }
                case 4:
                    system.displayAllOwners([](const Owner& owner) {
                        std::cout << "ID: " << owner.id << "\n"
                                << "Name: " << owner.name << "\n"
                                << "Phone: " << owner.phone << "\n"
                                << "Email: " << owner.email << "\n\n";
                    });
                    break;
                case 5:
                    system.displayAllVehicles([](const Vehicle& vehicle) {
                        displayVehicle(vehicle);
                    });
                    break;
                case 6:
                    system.displayAllRecords([](const ParkingRecord& record) {
                        displayRecord(record);
                    });
                    break;
                case 7: { // Advanced Search
                    std::vector<std::string> searchOptions = {
                        "\033[1;33mSearch by Owner Name\033[0m",
                        "\033[1;33mSearch by Vehicle Criteria\033[0m",
                        "\033[1;33mSearch by Time Range\033[0m",
                        "\033[1;33mSearch by Location\033[0m",
                        "\033[1;33mSearch by Plate and Date\033[0m",
                        "\033[1;33mBack to Main Menu\033[0m"
                    };

                    int searchChoice = InteractiveMenu::showSubMenu(searchOptions,"Advanced Search");
                    std::string input;
                    
                    switch (searchChoice) {
                        case 0: { // Search by Owner Name
                            std::cout << "Enter owner name: ";
                            std::getline(std::cin, input);
                            auto results = system.searchVehiclesByOwnerName(input);
                            for (const auto& vehicle : results) {
                                displayVehicle(vehicle);
                            }
                            break;
                        }
                        case 1: { // Search by Vehicle Criteria
                            std::string brand, model, color;
                            std::cout << "Enter brand (or press Enter to skip): ";
                            std::getline(std::cin, brand);
                            std::cout << "Enter model (or press Enter to skip): ";
                            std::getline(std::cin, model);
                            std::cout << "Enter color (or press Enter to skip): ";
                            std::getline(std::cin, color);
                            
                            auto results = system.searchVehiclesByAllCriteria(brand, model, color);
                            for (const auto& vehicle : results) {
                                displayVehicle(vehicle);
                            }
                            break;
                        }
                        case 2: { // Search by Time Range
                            std::string startTime, endTime;
                            std::cout << "Enter start time (YYYY-MM-DD HH:MM:SS): ";
                            std::getline(std::cin, startTime);
                            std::cout << "Enter end time (YYYY-MM-DD HH:MM:SS): ";
                            std::getline(std::cin, endTime);
                            
                            auto results = system.searchRecordsByTimeRange(startTime, endTime);
                            for (const auto& record : results) {
                                displayRecord(record);
                            }
                            break;
                        }
                        case 3: { // Search by Location
                            std::vector<std::string> locationOptions = {
                                "\033[1;33mNear Exit\033[0m",
                                "\033[1;33mNear Entrance\033[0m",
                                "\033[1;33mBack\033[0m"
                            };

                            int locChoice = InteractiveMenu::showSubMenu(locationOptions, "Search by Location");
                            if (locChoice < 2) {
                                auto results = system.searchVehiclesByLocation(locChoice == 0);
                                for (const auto& vehicle : results) {
                                    displayVehicle(vehicle);
                                }
                            }
                            break;
                        }
                        case 4: { // Search by Plate and Date
                            std::string plate, date;
                            std::cout << "Enter plate number: ";
                            std::getline(std::cin, plate);
                            std::cout << "Enter date (YYYY-MM-DD): ";
                            std::getline(std::cin, date);
                            
                            auto results = system.searchRecordsByPlateAndDate(plate, date);
                            for (const auto& record : results) {
                                displayRecord(record);
                            }
                            break;
                        }
                    }
                    if (searchChoice < 5) {
                        std::cout << "\nPress any key to continue...";
                        _getch();
                    }
                    break;
                }
                case 8: // Exit
                    return 0;
            }
            
            if (choice != 8) {
                std::cout << "\nPress any key to continue...";
                _getch();
            }
            
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            std::cout << "Press any key to continue...";
            _getch();
        }
    }
    
    return 0;
}