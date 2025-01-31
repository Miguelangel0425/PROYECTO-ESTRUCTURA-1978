#include "ParkingSystem.h"
#include <iostream>
#include <conio.h>
#include "InteractiveMenu.h"
#include "Validaciones.h"

// Add missing display functions
void displayVehicle(const Vehicle& vehicle) {
    std::cout << "Placa: " << vehicle.plate << "\n"
              << "Marca: " << vehicle.brand << "\n"
              << "Modelo: " << vehicle.model << "\n"
              << "Color: " << vehicle.color << "\n"
              << "Cedula del propietario: " << vehicle.ownerId << "\n\n";
}

void displayRecord(const ParkingRecord& record) {
    std::cout << "Placa: " << record.plate << "\n"
              << "Tiempo de entrada: " << record.entryTime;
    
    if (!record.exitTime.empty()) {
        std::cout << "\nTiempo de salida: " << record.exitTime;
    } else {
        std::cout << "\nVehículo aún en parqueadero";
    }
    std::cout << "\n\n";
}

int main() {
    ParkingSystem system(3);
    
    std::vector<std::string> mainMenuOptions = {
        "\033[1;33mAgregar Propietario\033[0m",            
        "\033[1;33mAgregar Vehiculo\033[0m",         
        "\033[1;33mRegistrar Entrada\033[0m",      
        "\033[1;33mRegistrar Salida\033[0m", 
        "\033[1;33mGrafica del Parquedero\033[0m",  // Nueva opción
        "\033[1;33mEncontrar ubicacion del vehiculo\033[0m",   // Nueva opción      
        "\033[1;33mLista de Propietarios\033[0m", 
        "\033[1;33mLista de Vehiculos\033[0m",
        "\033[1;33mLista de Registros\033[0m", 
        "\033[1;33mBusqueda Avanzada\033[0m", 
        "\033[1;33mActualizacion de datos\033[0m",
        "\033[1;33mEliminar Registros\033[0m",    
        "\033[1;33mSalir\033[0m"                 
    };
    
    InteractiveMenu mainMenu(mainMenuOptions,"Parking System");
    
    while (true) {
        std::system("cls");
        int choice = mainMenu.show();
        
        try {
            switch (choice) {
                case 0: {
                    Owner owner;
                    Validaciones validaciones;
                    while(true){
                        owner.id = Validaciones::ingresarTelefono("Ingrese el numero de cedula: ");
                        if(!validaciones.validarCedula(owner.id)){
                            std :: cout << "\033[31mNumero cedula invalido. Ingrese nuevamente\033[0m";
                            std :: cout << endl;
                        } else if(system.isOwnerRegistered(owner.id)) {
                            std::cout << "\033[31mEl Propietario ya se encuentra registrado. Ingrese nuevamente\033[0m" << std::endl;
                        } else {
                            break;
                        }
                    }
                    owner.name = Validaciones::ingresarString("Ingrese el nombre: ");
                    while(!validaciones.validarCelularEcuador(owner.phone)){
                        owner.phone = Validaciones::ingresarTelefono("Ingrese el telefono: "); // Changed from ingresarTelefono which doesn't exist
                        if(!validaciones.validarCelularEcuador(owner.phone)){
                            std :: cout << "\033[31mNumero celular invalido. Ingrese nuevamente\033[0m";
                            std :: cout << endl;
                        }
                    }
                    while(!validaciones.validarCorreo(owner.email)){
                        owner.email = Validaciones::ingresarCorreo("Ingrese el correo: ");
                        if(!validaciones.validarCorreo(owner.email)){
                            std :: cout << "\033[31mCorreo invalido. Ingrese nuevamente\033[0m";
                            std :: cout << endl;
                        }
                    }
                    
    
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
                    Validaciones validaciones;
                    while(!validaciones.validarPlaca(vehicle.plate)){
                        vehicle.plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo: ");
                        if(!validaciones.validarPlaca(vehicle.plate)){
                            std::cout << "\033[31mPlaca invalida. Ingrese nuevamente\033[0m" << std::endl;
                            std :: cout << endl;
                        }
                    }
                    if(system.isPlateRegistered(vehicle.plate)) {
                        throw std::runtime_error("El vehiculo ya se encuentra registrado.");
                    }
                    vehicle.brand = Validaciones::ingresarString("Ingrese la marca del vehiculo: ");
                    
                    vehicle.model = Validaciones::ingresarString("Ingrese el modelo del vehiculo: ");
                    
                    vehicle.color = Validaciones::ingresarString("Ingrese el color del vehiculo: ");
                    
                    while (true) {
                     
                        vehicle.ownerId = Validaciones::ingresarTelefono("Ingrese el numero de cedula del propietario: ");
                        if(!validaciones.validarCedula(vehicle.ownerId)){
                            std::cout << "\033[31mCedula invalida. Ingrese nuevamente.\033[0m" << std::endl;
                        } else if (system.findOwner(vehicle.ownerId)) {
                            system.addVehicle(vehicle);
                            break;
                        } else {
                            Owner owner;
                            owner.id = vehicle.ownerId; // Mostrar el ID ingresado anteriormente
                            std::cout << "\033[31mCedula no se encuentra registrada:\033[0m" <<  std::endl;
                            std::cout << "\033[31mPara modificar o agregar propietario.\033[0m" << std::endl;
                            std::cout << "\033[31mPresione enter.\033[0m" << std::endl;
                            std::cin.ignore(); // Esperar a que el usuario presione Enter
                               
                            std::vector<std::string> idErrorOptions = {
                                "\033[1;33mReingresar Cedula\033[0m",
                                "\033[1;33mAgregar Nuevo Propietario\033[0m",
                                "\033[1;33mCancelar\033[0m"
                            };
                            
                            int idErrorChoice = InteractiveMenu::showSubMenu(idErrorOptions, "Invalid Owner ID");
                            
                            if (idErrorChoice == 0) {
                                continue;
                            } else if (idErrorChoice == 1) {
                                Owner owner;
                                Validaciones validaciones;
                                while(true){
                                    owner.id = vehicle.ownerId; // Mostrar el ID ingresado anteriormente
                                    std::cout << "Use la cedula: " << owner.id << std::endl;
                                    owner.id = Validaciones::ingresarTelefono("Ingrese el numero de cedula: ");
                                    if (!validaciones.validarCedula(owner.id)){
                                        std :: cout << "\033[31mCedula invalida. Ingrese nuevamente\033[0m";
                                        std :: cout << endl;
                                    } else {
                                        break;
                                    }
                                    
                                }           
                                owner.name = Validaciones::ingresarString("Ingrese el nombre: ");
                                while(!validaciones.validarCelularEcuador(owner.phone)){
                                    owner.phone = Validaciones::ingresarTelefono("Ingrese el telefono: "); // Changed from ingresarTelefono which doesn't exist
                                    if(!validaciones.validarCelularEcuador(owner.phone)){
                                        std :: cout << "\033[31mNumero celular invalido. Ingrese nuevamente\033[0m";
                                        std :: cout << endl;
                                    }
                                }
                                while(!validaciones.validarCorreo(owner.email)){
                                    owner.email = Validaciones::ingresarCorreo("Ingrese el correo: ");
                                    if(!validaciones.validarCorreo(owner.email)){
                                    std :: cout << "\033[31mCorreo invalido. Ingrese nuevamente\033[0m";
                                    std :: cout << endl;
                                    }
                                }
                                
                                if (Owner::isValid(owner)) {
                                    system.addOwner(owner);
                                    vehicle.ownerId = owner.id;
                                    system.addVehicle(vehicle);
                                    break;
                                } else {
                                    std::cout << "\033[31mDatos de propietario inválidos. Cancelando operación.\033[0m" << std::endl;
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
                    Vehicle vehicle;

                    vehicle.plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo: ");
                    Validaciones validaciones;
                    if(!validaciones.validarPlaca(vehicle.plate)){
                        std::cout << "\033[31mPlaca invalida\033[0m" << std::endl;
                        std :: cout << endl;
                        break;
                    }
                    system.registerEntry(vehicle.plate);
                    break;
                }
                case 3: {
                Vehicle vehicle;
                    vehicle.plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo: ");
                    Validaciones validaciones;
                    if(!validaciones.validarPlaca(vehicle.plate)){
                        std::cout << "\033[31mPlaca invalida\033[0m" << std::endl;
                        std :: cout << endl;
                        break;
                    }
                    system.registerExit(vehicle.plate);
                    break;
                }

                case 4: { // Display Parking Layout
                        system.displayParkingLayout();
                        break;
                    }   
                case 5: { // Find Vehicle Location
                    Vehicle vehicle;

                    vehicle.plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo: ");
                    Validaciones validaciones;
                    if(!validaciones.validarPlaca(vehicle.plate)){
                        std::cout << "\033[31mPlaca invalida\033[0m" << std::endl;
                        std :: cout << endl;
                        break;
                    }
                    system.getVehicleLocation(vehicle.plate);
                    break;
                    }         
                case 6:
                    system.displayAllOwners([](const Owner& owner) {
                        std::cout << "Cedula: " << owner.id << "\n"
                                << "Nombre: " << owner.name << "\n"
                                << "Telefono: " << owner.phone << "\n"
                                << "Correo Electronico: " << owner.email << "\n\n";
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
                        "\033[1;33mBuscar por Nombre de Usuario\033[0m",
                        "\033[1;33mBuscar por Criterio de Vehiculo\033[0m",
                        "\033[1;33mBuscar por Rango de Tiempo\033[0m",
                        "\033[1;33mBuscar por Locacion\033[0m",
                        "\033[1;33mBuscar por placa y fecha\033[0m",
                        "\033[1;33mRegresar al Menu Principal\033[0m"
                    };

                    int searchChoice = InteractiveMenu::showSubMenu(searchOptions,"Advanced Search");
                    std::string input;
                    
                    switch (searchChoice) {
                        case 0: { // Search by Owner Name
                            input = Validaciones :: ingresarString("Ingrese el Nombre de Usuario a buscar: ");
                            auto results = system.searchVehiclesByOwnerName(input);
                            for (const auto& vehicle : results) {
                                displayVehicle(vehicle);
                            }
                            break;
                        }
                        case 1: { // Search by Vehicle Criteria
                            std::string brand, model, color;
                            brand = Validaciones::ingresarString("Ingresar marca: ");
                            model = Validaciones::ingresarString("Ingresar Modelo: ");
                            color = Validaciones::ingresarString("Ingresar color: ");
                            auto results = system.searchVehiclesByAllCriteria(brand, model, color);
                            for (const auto& vehicle : results) {
                                displayVehicle(vehicle);
                            }
                            break;
                        }
                        case 2: { // Search by Time Range
                            Validaciones validaciones;
                            std::string startTime, endTime;
                            while(!validaciones.validarFechaHora(startTime)){
                                startTime = Validaciones::ingresarFecha("Ingrese tiempo de inicio (YYYY-MM-DD HH:MM:SS): ");
                                if (!validaciones.validarFechaHora(startTime)){
                                    std::cout << "\033[31mRango invalido. Ingrese nuevamente\033[0m";
                                    std::cout << endl;
                                }
                            }
                            while(!validaciones.validarFechaHora(endTime)){
                                endTime = Validaciones::ingresarFecha("Ingrese tiempo de fin (YYYY-MM-DD HH:MM:SS): ");
                                if (!validaciones.validarFechaHora(endTime)){
                                    std::cout << "\033[31mRango invalido. Ingrese nuevamente\033[0m";
                                    std::cout << endl;
                                }
                            }

                            auto results = system.searchRecordsByTimeRange(startTime, endTime);
                            for (const auto& record : results) {
                                displayRecord(record);
                            }
                            break;
                        }
                        case 3: { // Search by Location
                            std::vector<std::string> locationOptions = {
                                "\033[1;33mSalida Cercana\033[0m",
                                "\033[1;33mEntrada Cercana\033[0m",
                                "\033[1;33mRegresar al Menu Principal\033[0m"
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
                            Vehicle vehicle;

                            vehicle.plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo: ");
                            Validaciones validaciones;
                            if(!validaciones.validarPlaca(vehicle.plate)){
                                std::cout << "\033[31mPlaca invalida\033[0m" << std::endl;
                                std :: cout << endl;
                                break;
                            }
                
                            std::getline(std::cin, plate);
                            std::cout << "Ingrese Fecha (YYYY-MM-DD): ";
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
                        "\033[1;33mActualizar Propietario\033[0m",
                        "\033[1;33mActualizar Vehiculo\033[0m",
                        "\033[1;33mSalir\033[0m"
                    };
                    int updateChoice = InteractiveMenu::showSubMenu(updateOptions, "Update Records");
                    switch (updateChoice) {
                        case 0: {
                    Validaciones validaciones;
                    std::vector<std::string> updateOptions = {
                        "\033[1;33mActualice Nombre\033[0m",
                        "\033[1;33mActualice Numero de Telefono\033[0m",
                        "\033[1;33mActualice Correo Electronico\033[0m",
                        "\033[1;33mActualice Toda la Informacion\033[0m"
                        "\033[1;33mVolver \033[0m"
                    };

                    std::string id;
                    while(true){
                        id = Validaciones::ingresarTelefono("Ingrese el numero de cedula: ");
                        if (!validaciones.validarCedula(id)){
                            std :: cout << "\033[31mCedula invalida. Ingrese nuevamente\033[0m";
                            std :: cout << endl;
                        } else {
                            break;
                        }
                    }
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
                            Validaciones validaciones;
                            std::cout << "Telefono actual: " << existingOwner->phone << std::endl;
                            do{
                                updatedOwner.phone = Validaciones::ingresarTelefono("Ingrese el nuevo numero de telefono: "); // Changed from ingresarTelefono which doesn't exist
                                if(!validaciones.validarCelularEcuador(updatedOwner.phone)){
                                    std :: cout << "\033[31mNumero celular invalido. Ingrese nuevamente\033[0m";
                                    std :: cout << endl;
                                }
                            }while(!validaciones.validarCelularEcuador(updatedOwner.phone));

                            
                            break;
                        }
                        case 2:{
                            Validaciones validaciones;
                            std::cout << "Correo actual: " << existingOwner->email << std::endl;
                            do{
                                updatedOwner.email = Validaciones::ingresarCorreo("Ingrese el nuevo correo: ");
                                if(!validaciones.validarCorreo(updatedOwner.email)){
                                    std :: cout << "\033[31mCorreo invalido. Ingrese nuevamente\033[0m";
                                    std :: cout << endl;
                                }
                            }while(!validaciones.validarCorreo(updatedOwner.email));

                            
                            break;
                        }
                        case 3:{
                            Validaciones validaciones;
                            std::cout << "Datos actuales: " << std::endl;
                            std::cout << "Nombre: " << existingOwner->name << std::endl;
                            std::cout << "Telefono: " << existingOwner->phone << std::endl;
                            std::cout << "Correo: " << existingOwner->email << std::endl;
                            updatedOwner.name = Validaciones::ingresarString("Ingrese el nuevo nombre: ");
                            
                            do{
                                updatedOwner.phone = Validaciones::ingresarTelefono("Ingrese el nuevo numero de telefono: "); // Changed from ingresarTelefono which doesn't exist
                                if(!validaciones.validarCelularEcuador(updatedOwner.phone)){
                                    std :: cout << "\033[31mNumero celular invalido. Ingrese nuevamente\033[0m";
                                    std :: cout << endl;
                                }
                            }while(!validaciones.validarCelularEcuador(updatedOwner.phone));

                            do{
                                updatedOwner.email = Validaciones::ingresarCorreo("Ingrese el nuevo correo: ");
                                if(!validaciones.validarCorreo(updatedOwner.email)){
                                    std :: cout << "\033[31mCorreo invalido. Ingrese nuevamente\033[0m";
                                    std :: cout << endl;
                                }
                            }while(!validaciones.validarCorreo(updatedOwner.email));
                            break;
                        }
                        case 4:{
                            delete existingOwner;
                            break;
                        }
                    }

                    if(!(updateChoice == 4)){
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
                        case 1: {
            std::string plate;
            Validaciones validaciones;
            while(!validaciones.validarPlaca(plate)){
                plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo a actualizar: ");
                if(!validaciones.validarPlaca(plate)){
                    std::cout << "\033[31mPlaca invalida. Ingrese nuevamente\033[0m" << std::endl;
                    std::cout << endl;
                }
            }
            
            Vehicle* existingVehicle = system.findVehicle(plate);
            if (!existingVehicle) {
                std::cout << "Vehiculo no encontrado" << std::endl;
                break;
            }

            Vehicle updatedVehicle = *existingVehicle;
            
            std::vector<std::string> vehicleUpdateOptions = {
                "\033[1;33mActualizar Marca\033[0m",
                "\033[1;33mActualizar Modelo\033[0m",
                "\033[1;33mActualizar Color\033[0m",
                "\033[1;33mActualizar Toda la Informacion\033[0m",
                "\033[1;33mSalir\033[0m"
            };

            int vehicleUpdateChoice = InteractiveMenu::showSubMenu(vehicleUpdateOptions, "Update Vehicle");
            
            switch(vehicleUpdateChoice) {
                case 0:
                    std::cout << "Marca actual: " << existingVehicle->brand << std::endl;
                    updatedVehicle.brand = Validaciones::ingresarString("Ingrese la nueva marca: ");
                    break;
                case 1:
                    std::cout << "Modelo actual: " << existingVehicle->model << std::endl;
                    updatedVehicle.model = Validaciones::ingresarString("Ingrese el nuevo modelo: ");
                    break;
                case 2:
                    std::cout << "Color actual: " << existingVehicle->color << std::endl;
                    updatedVehicle.color = Validaciones::ingresarString("Ingrese el nuevo color: ");
                    break;
                case 3:
                    std::cout << "Datos actuales:" << std::endl;
                    std::cout << "Marca: " << existingVehicle->brand << std::endl;
                    std::cout << "Modelo: " << existingVehicle->model << std::endl;
                    std::cout << "Color: " << existingVehicle->color << std::endl;
                    
                    updatedVehicle.brand = Validaciones::ingresarString("Ingrese la nueva marca: ");
                    updatedVehicle.model = Validaciones::ingresarString("Ingrese el nuevo modelo: ");
                    updatedVehicle.color = Validaciones::ingresarString("Ingrese el nuevo color: ");
                    break;
                case 4:
                    delete existingVehicle;
                    break;
            }

            if (vehicleUpdateChoice != 4) {
                if (system.updateVehicle(updatedVehicle)) {
                    std::cout << "Vehiculo actualizado exitosamente" << std::endl;
                } else {
                    std::cout << "Error al actualizar el vehiculo" << std::endl;
                }
            }
            
            delete existingVehicle;
            break;
            }
            }
            break;   
                };

                case 11: {
                    std::vector<std::string> deleteOptions = {
                        "\033[1;33mEliminar Propietario\033[0m",
                        "\033[1;33mEliminar Vehiculo\033[0m",
                        "\033[1;33mSalir\033[0m"
                    };  
                    int deleteChoice = InteractiveMenu::showSubMenu(deleteOptions, "Delete Records");
                    switch (deleteChoice){
                        case 0:{
                            Validaciones validaciones;
                            std::string id;
                            while (!validaciones.validarCedula(id)){
                                id = Validaciones::ingresarTelefono("Ingrese el numero de cedula del propietario a eliminar:");
                                if(!validaciones.validarCedula(id)){
                                    std :: cout << "\033[31mCedula invalida. Ingrese nuevamente\033[0m";
                                    std :: cout << endl;    
                                }
                            }
                            if(system.deleteOwner(id)){
                                std::cout << "Propietario eliminado exitosamente" << std::endl;
                            } else {
                                std::cout << "Propietario no encontrado" << std::endl;
                            }
                            break;
                        }
                        case 1:{
                            std::string plate;
                            Validaciones validaciones;
                            while(!validaciones.validarPlaca(plate)){
                                plate = Validaciones::ingresarPlaca("Ingrese la placa del vehiculo a eliminar: ");
                                if(!validaciones.validarPlaca(plate)){
                                    std::cout << "\033[31mPlaca invalida. Ingrese nuevamente\033[0m" << std::endl;
                                    std :: cout << endl;
                                }
                            }
                            
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