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

        if (isdigit(c) || c == '-') {
            if (fecha.length() < 10) {
                fecha += c;
                cout << c;
            }
            else {
                cout << "\a";
            }
        }
        else if (c == '\b' && !fecha.empty()) {
            fecha.pop_back();
            cout << "\b \b";
        }
        else if (c == '\r') {
            if (fecha.length() == 10) { // formato YYYY-MM-DD
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
            if (validarCorreo(correo)) {
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
    return correo;
}

bool Validaciones::validarCedula(const string& cedula) {
    if (cedula.length() != 10) {
        return false;
    }

    int suma = 0;
    for (int i = 0; i < 9; i++) {
        int digito = cedula[i] - '0';
        if (i % 2 == 0) {
            digito *= 2;
            if (digito > 9) {
                digito -= 9;
            }
        }
        suma += digito;
    }

    int ultimoDigito = cedula[9] - '0';
    int digitoVerificador = (10 - (suma % 10)) % 10;

    return ultimoDigito == digitoVerificador;
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

bool Validaciones::validarCorreo(const string& correo) {
    const regex pattern(R"((\w+)(\.{1}\w+)*@(\w+)(\.{1}\w+)+)");
    return regex_match(correo, pattern);
}
