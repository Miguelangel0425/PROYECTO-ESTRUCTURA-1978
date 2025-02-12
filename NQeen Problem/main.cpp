#include "NQueens.h"
#include <iostream>

int main() {
    int n;
    std::cout << "Ingrese el tamanio del tablero (N x N): ";
    std::cin >> n;

    if (n < 4) {
        std::cout << "El tamanio debe ser al menos 4x4 para tener solucion.\n";
        return 1;
    }

    NQueens queens(n);
    
    std::cout << "Resolviendo el problema de " << n << " reinas...\n";
    std::cout << "La ventana grafica se abrira automaticamente.\n";
    std::cout << "El proceso se registrara en 'queens_steps.txt'\n\n";
    
    if (queens.solveNQueens(0)) {
        std::cout << "¡Solucion encontrada!\n";
        std::cout << "Numero total de pasos: " << queens.getSteps() << "\n";
        std::cout << "Maximo numero de pasos en una rama: " << queens.getMaxSteps() << "\n";
        queens.run(); // Mantener la ventana abierta con la solución final
    } else {
        std::cout << "No se encontro solucion para " << n << " reinas.\n";
    }

    return 0;
}