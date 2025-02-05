#include "Validaciones.h"
#include <conio.h>
#include <regex>
#include <iostream>

string Validaciones::ingresarString(const string& msj) {
    string input;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isalpha(c) || c == ' ') {
            input += c;
            cout << c;
        }
        else if (c == '\b' && !input.empty()) {
            input.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            if (!input.empty()) {
                break;
            }
            else {
                cout << "\a";
            }
        }
        else {
            cout << "\a";
        }
    }

    cout << endl;
    return input;
}

string Validaciones::ingresarNumero(const string& msj) {
    string input;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isdigit(c)) {  
            input += c;
            cout << c;
        }
        else if (c == '\b' && !input.empty()) {  
            input.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {  
            if (!input.empty()) {
                break;
            }
            else {
                cout << "\a";
            }
        }
        else {
            cout << "\a";  
        }
    }

    cout << endl;
    return input;
}


string Validaciones::ingresarCedula(const string& msj) {
    string cedula;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isdigit(c)) {
            if (cedula.length() < 10) {
                cedula += c;
                cout << c;
            }
            else {
                cout << "\a";
            }
        }
        else if (c == '\b' && !cedula.empty()) {
            cedula.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            if (cedula.length() == 10 && validarCedula(cedula)) {
                break;
            }
            else {
                cout << "\a";
            }
        }
        else {
            cout << "\a";
        }
    }

    cout << endl;
    return cedula;
}

string Validaciones::ingresarPlaca(const string& msj) {
    string placa;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isalnum(c)) {
            if (placa.length() < 7) {
                placa += toupper(c);
                cout << (char)toupper(c);
            }
            else {
                cout << "\a";
            }
        }
        else if (c == '\b' && !placa.empty()) {
            placa.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            if (placa.length() == 7) {
                break;
            }
            else {
                cout << "\a";
            }
        }
        else {
            cout << "\a";
        }
    }

    cout << endl;
    return placa;
}

string Validaciones::ingresarTelefono(const string& msj) {
    string telefono;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isdigit(c)) {
            if (telefono.length() < 10) {
                telefono += c;
                cout << c;
            }
            else {
                cout << "\a";
            }
        }
        else if (c == '\b' && !telefono.empty()) {
            telefono.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            if (telefono.length() == 10) {
                break;
            }
            else {
                cout << "\a";
            }
        }
        else {
            cout << "\a";
        }
    }

    cout << endl;
    return telefono;
}

string Validaciones::ingresarFecha(const string& msj) {
    string fecha;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isdigit(c) || c == '-' || c == ' '|| c == ':') {  
            fecha += c;
            cout << c;
        }
        else if (c == '\b' && !fecha.empty()) {  
            fecha.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {  
            break;
        }
        else {
            cout << "\a";  
        }
    }

    cout << endl;
    return fecha;
}

string Validaciones::ingresarHora(const string& msj) {
    string hora;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isdigit(c) || c == ':') {
            if (hora.length() < 8) {
                hora += c;
                cout << c;
            }
            else {
                cout << "\a";
            }
        }
        else if (c == '\b' && !hora.empty()) {
            hora.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            if (hora.length() == 8) { // formato HH:MM:SS
                break;
            }
            else {
                cout << "\a";
            }
        }
        else {
            cout << "\a";
        }
    }

    cout << endl;
    return hora;
}

string Validaciones::ingresarCorreo(const string& msj) {
    string correo;
    char c;

    cout << msj;

    while (true) {
        c = _getch();

        if (isalnum(c) || c == '@' || c == '.' || c == '_' || c == '-') {
            correo += c;
            cout << c;
        }
        else if (c == '\b' && !correo.empty()) {
            correo.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            break;
        }
        else {
            cout << "\a";
        }
    }

    cout << endl;
    return correo;
}

bool Validaciones::validarCedula(const string& idInput) {
    if ((idInput.length() != 10) || (idInput == "0000000000")) {
        return false;
    }

    long id;
    try {
        id = std::stol(idInput);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    std::vector<long> digits(10);
    long remainder;
    long doubledDigit;
    long evenPositionSum = 0;
    long oddPositionSum = 0;
    long totalSum;
    long checkDigit;

    for (int i = 9; i >= 0; --i) {
        digits[i] = id % 10;
        id /= 10;
    }

    for (int i = 0; i < 9; i += 2) {
        doubledDigit = digits[i] * 2;
        if (doubledDigit > 9) {
            doubledDigit -= 9;
        }
        evenPositionSum += doubledDigit;
    }

    for (int i = 1; i < 9; i += 2) {
        oddPositionSum += digits[i];
    }

    totalSum = evenPositionSum + oddPositionSum;
    remainder = totalSum % 10;
    checkDigit = 10 - remainder;
    if (checkDigit == 10) {
        checkDigit = 0;
    }

    return checkDigit == digits[9];
}

bool Validaciones::validarPlaca(const string& placa) {
      // Códigos de provincias válidos
    const std::vector<std::string> provinceCodes = {
        "A", "W", "Q", "B", "G", "S", "U", "I", "P", 
        "C", "L", "Y", "H", "R", "J", "X", "M", "K", 
        "O", "V", "T", "E", "N", "Z"
    };

    // Códigos especiales
    const std::vector<std::string> specialCodes = {
        "CC", "CD", "OI", "AT", "IT"
    };

    // Convertir a mayúsculas para validación consistente
    std::string upperPlate = placa;
    std::transform(upperPlate.begin(), upperPlate.end(), upperPlate.begin(), ::toupper);

    // Validar placas de provincias
    if (upperPlate.length() == 6 || upperPlate.length() == 7) {
        // Verificar si el primer código es un código de provincia válido
        auto it = std::find(provinceCodes.begin(), provinceCodes.end(), upperPlate.substr(0, 1));
        if (it != provinceCodes.end()) {
            // Verificar que los siguientes 2 caracteres sean letras
            for (int i = 1; i < 3; ++i) {
                if (!std::isalpha(upperPlate[i])) {
                    return false;
                }
            }
            
            // Verificar que los últimos 3-4 caracteres sean números
            for (size_t i = 3; i < upperPlate.length(); ++i) {
                if (!std::isdigit(upperPlate[i])) {
                    return false;
                }
            }
            return true;
        }
    }

    // Validar placas especiales
    if (upperPlate.length() >= 4) {
        auto it = std::find(specialCodes.begin(), specialCodes.end(), upperPlate.substr(0, 2));
        if (it != specialCodes.end()) {
            // Verificar que hay números después del código especial
            for (size_t i = 2; i < upperPlate.length(); ++i) {
                if (!std::isdigit(upperPlate[i])) {
                    return false;
                }
            }
            return true;
        }
    }

    // Verificar placas de internación temporal con color
    if (upperPlate.length() >= 8) {
        if ((upperPlate.substr(0, 2) == "IT") && 
            (upperPlate.substr(2, 4) == "-AZUL" || upperPlate.substr(2, 4) == "-ROJO")) {
            for (size_t i = 6; i < upperPlate.length(); ++i) {
                if (!std::isdigit(upperPlate[i])) {
                    return false;
                }
            }
            return true;
        }
    }

    return false;

}
bool Validaciones::validarCelularEcuador(const string& numero) {
    
    regex patron("^09[5-9][0-9]{7}$");
    return regex_match(numero, patron);
}
bool Validaciones::validarCorreo(const string& correo) {
    const regex pattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.{1}\w+)+)");
    return regex_match(correo, pattern);
}

bool Validaciones::esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

bool Validaciones::validarFechaHora(const std::string& fechaHora) {
    std::regex patron(R"(^(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})$)");
    std::smatch match;

    if (!std::regex_match(fechaHora, match, patron)) {
        return false;
    }

    int anio = std::stoi(match[1].str());
    int mes = std::stoi(match[2].str());
    int dia = std::stoi(match[3].str());
    int hora = std::stoi(match[4].str());
    int minuto = std::stoi(match[5].str());
    int segundo = std::stoi(match[6].str());

    // Año mínimo permitido
    if (anio < 1900 || anio > 2100) { 
        return false;
    }

    if (mes < 1 || mes > 12 || dia < 1 || hora > 23 || minuto > 59 || segundo > 59) {
        return false;
    }

    int diasPorMes[] = {0, 31, (esBisiesto(anio) ? 29 : 28), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return dia <= diasPorMes[mes];
}
