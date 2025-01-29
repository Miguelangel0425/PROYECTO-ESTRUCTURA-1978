#pragma once
#include <string>

using namespace std;

class Validaciones {
public:
    static string ingresarString(const string& msj);
    static bool validarCedula(const string& cedula);
    static bool validarCorreo(const string& correo);
    static string ingresarCedula(const string& msj);
    static string ingresarCorreo(const string& msj);
    static string ingresarNumero(const string& msj);
    // Nuevas validaciones específicas para el sistema de parqueo
    static string ingresarPlaca(const string& msj);
    static string ingresarTelefono(const string& msj);
    static string ingresarFecha(const string& msj);
    static string ingresarHora(const string& msj);
    bool validarPlaca(const string &placa);
    bool validarCelularEcuador(const string& numero);
};
