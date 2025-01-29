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
        "\033[1;33mDisplay Parking Layout\033[0m",  // Nueva opción
        "\033[1;33mFind Vehicle Location\033[0m",   // Nueva opción      
        "\033[1;33mDisplay All Owners\033[0m", 
        "\033[1;33mDisplay All Vehicles\033[0m",
        "\033[1;33mDisplay All Records\033[0m", 
        "\033[1;33mAdvanced Search\033[0m", 
        "\033[1;33mUpdate Owner\033[0m",
        "\033[1;33mDelete Records\033[0m",    
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
                    
                    vehicle.plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo: ");
                    Validaciones validaciones;
                    if(!validaciones.validarPlaca(vehicle.plate)){
                        std::cout << "Placa inválida" << std::endl;
                        break;
                    }
                   
                    vehicle.brand = Validaciones::ingresarString("Ingrese la marca del vehiculo: ");
                    
                    vehicle.model = Validaciones::ingresarString("Ingrese el modelo del vehiculo: ");
                    
                    vehicle.color = Validaciones::ingresarString("Ingrese el color del vehiculo: ");
                    
                    while (true) {
                     
                        vehicle.ownerId = Validaciones::ingresarCedula("Ingrese el numero de cedula del propietario: ");
                        if(!validaciones.validarCedula(vehicle.ownerId)){
                            std::cout << "Cedula inválida" << std::endl;
                        } else if (system.findOwner(vehicle.ownerId)) {
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

                case 4: { // Display Parking Layout
                        system.displayParkingLayout();
                        break;
                    }   
                case 5: { // Find Vehicle Location
                    std::string plate;
                    std::cout << "Enter plate number: ";
                    std::getline(std::cin, plate);
                    system.getVehicleLocation(plate);
                    break;
                    }         
                case 6:
                    system.displayAllOwners([](const Owner& owner) {
                        std::cout << "ID: " << owner.id << "\n"
                                << "Name: " << owner.name << "\n"
                                << "Phone: " << owner.phone << "\n"
                                << "Email: " << owner.email << "\n\n";
                    });
                    break;
                case 7:
                    system.displayAllVehicles([](const Vehicle& vehicle) {
                        displayVehicle(vehicle);
                    });
                    break;
                case 8:
                    system.displayAllRecords([](const ParkingRecord& record) {
                        displayRecord(record);
                    });
                    break;
                case 9: { // Advanced Search
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
                case 10: {
                    std::vector<std::string> updateOptions = {
                        "\033[1;33mUpdate Name\033[0m",
                        "\033[1;33mUpdate Phone Number\033[0m",
                        "\033[1;33mUpdate Email\033[0m",
                        "\033[1;33mUpdate All Data\033[0m"
                        "\033[1;33mBack\033[0m"
                    };

                    std::string id = Validaciones::ingresarCedula("Ingrese el numero de cedula del propietario a actualizar:");
                    Owner* existingOwner = system.findOwner(id);

                    if (!existingOwner){
                        std::cout << "Propietario no encontrado" << std::endl;
                        break;
                    }

                    Owner updatedOwner = *existingOwner;

                    int updateChoice = InteractiveMenu::showSubMenu(updateOptions, "Update Owner");

                    switch (updateChoice){
                        case 0:{
                            std::cout << "Nombre actual: " << existingOwner->name << std::endl;
                            updatedOwner.name = Validaciones::ingresarString("Ingrese el nuevo nombre: ");
                            break;
                        }
                        case 1:{
                            std::cout << "Telefono actual: " << existingOwner->phone << std::endl;
                            updatedOwner.phone = Validaciones::ingresarTelefono("Ingrese el nuevo telefono: ");
                            break;
                        }
                        case 2:{
                            std::cout << "Correo actual: " << existingOwner->email << std::endl;
                            updatedOwner.email = Validaciones::ingresarCorreo("Ingrese el nuevo correo: ");
                            break;
                        }
                        case 3:{
                            std::cout << "Datos actuales: " << std::endl;
                            std::cout << "Nombre: " << existingOwner->name << std::endl;
                            std::cout << "Telefono: " << existingOwner->phone << std::endl;
                            std::cout << "Correo: " << existingOwner->email << std::endl;
                            updatedOwner.name = Validaciones::ingresarString("Ingrese el nuevo nombre: ");
                            updatedOwner.phone = Validaciones::ingresarTelefono("Ingrese el nuevo telefono: ");
                            updatedOwner.email = Validaciones::ingresarCorreo("Ingrese el nuevo correo: ");
                            break;
                        }
                        case 4:{
                            delete existingOwner;
                            break;
                        }
                    }

                    if(!updateChoice == 4){
                        if(Owner::isValid(updatedOwner)){
                            system.updateOwner(updatedOwner);
                            std::cout << "Propietario actualizado exitosamente" << std::endl;
                        } else {
                            throw std::runtime_error("Datos de propietario inválidos");
                        }
                    };

                    delete existingOwner;
                    break;
                }

                case 11: {
                    std::vector<std::string> deleteOptions = {
                        "\033[1;33mDelete Owner\033[0m",
                        "\033[1;33mDelete Vehicle\033[0m",
                        "\033[1;33mExit\033[0m"
                    };  
                    int deleteChoice = InteractiveMenu::showSubMenu(deleteOptions, "Delete Records");
                    switch (deleteChoice){
                        case 0:{
                            std::string id = Validaciones::ingresarCedula("Ingrese el numero de cedula del propietario a eliminar:");
                            if(system.deleteOwner(id)){
                                std::cout << "Propietario eliminado exitosamente" << std::endl;
                            } else {
                                std::cout << "Propietario no encontrado" << std::endl;
                            }
                            break;
                        }
                        case 1:{
                            std::string plate;
                            std::cout << "Ingrese la placa del vehiculo a eliminar: ";
                            std::getline(std::cin, plate);
                            if(system.deleteVehicle(plate)){
                                std::cout << "Vehiculo eliminado exitosamente" << std::endl;
                            } else {
                                std::cout << "Vehiculo no encontrado" << std::endl;
                            }
                            break;
                        }
                    }
                    break;
                } 

                case 12: // Exit
                    return 0;
            }
        
            if (choice != 12) {
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