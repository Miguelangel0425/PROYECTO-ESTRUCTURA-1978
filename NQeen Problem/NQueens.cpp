#include "NQueens.h"
#include <iostream>
#include <thread>
#include <chrono>

NQueens::NQueens(int size) : n(size), steps(0), maxSteps(0) {
    // Inicializar tablero
    board = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    logFile.open("queens_steps.txt");
    
    // Crear ventana
    int windowSize = (CELL_SIZE * n) + (2 * WINDOW_PADDING);
    window = new sf::RenderWindow(sf::VideoMode(windowSize, windowSize), "N Queens Solver");
    window->setFramerateLimit(60);
    
    // Cargar textura de la reina
    if (!queenTexture.loadFromFile("queen.png")) {
        std::cout << "Error cargando la imagen de la reina" << std::endl;
        exit(1);
    }
    
    // Configurar sprite de la reina
    queenSprite.setTexture(queenTexture);
    float scale = static_cast<float>(CELL_SIZE) / queenTexture.getSize().x * 0.8f;
    queenSprite.setScale(scale, scale);
    
    // Configurar forma de celda
    cellShape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    
    clearBoard();
}

NQueens::~NQueens() {
    if (logFile.is_open()) {
        logFile.close();
    }
    delete window;
}

void NQueens::clearBoard() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            board[i][j] = 0;
        }
    }
}

void NQueens::drawBoard() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cellShape.setPosition(WINDOW_PADDING + j * CELL_SIZE, WINDOW_PADDING + i * CELL_SIZE);
            
            if ((i + j) % 2 == 0) {
                cellShape.setFillColor(sf::Color(240, 217, 181)); // Color claro
            } else {
                cellShape.setFillColor(sf::Color(181, 136, 99)); // Color oscuro
            }
            
            window->draw(cellShape);
        }
    }
}

void NQueens::drawQueens() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i][j] == 1) {
                queenSprite.setPosition(
                    WINDOW_PADDING + j * CELL_SIZE + (CELL_SIZE - queenSprite.getGlobalBounds().width) / 2,
                    WINDOW_PADDING + i * CELL_SIZE + (CELL_SIZE - queenSprite.getGlobalBounds().height) / 2
                );
                window->draw(queenSprite);
            }
        }
    }
}

void NQueens::handleEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            exit(0);
        }
    }
}

bool NQueens::isSafe(int row, int col) {
    for (int i = 0; i < col; i++) {
        if (board[row][i] == 1) return false;
    }
    
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--) {
        if (board[i][j] == 1) return false;
    }
    
    for (int i = row, j = col; i < n && j >= 0; i++, j--) {
        if (board[i][j] == 1) return false;
    }
    
    return true;
}

void NQueens::printBoardToFile(int row, int col) {
    steps++;
    maxSteps = std::max(maxSteps, steps);
    
    logFile << "Paso " << steps << ":\n";
    logFile << "Colocando reina en posición (" << row + 1 << "," << col + 1 << ")\n";
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            logFile << (board[i][j] ? "Q " : ". ");
        }
        logFile << "\n";
    }
    logFile << "\n";
}

bool NQueens::solveNQueens(int col) {
    if (col >= n) return true;

    for (int i = 0; i < n; i++) {
        if (isSafe(i, col)) {
            board[i][col] = 1;
            printBoardToFile(i, col);

            // Actualizar visualización
            window->clear(sf::Color::White);
            drawBoard();
            drawQueens();
            window->display();
            
            handleEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (solveNQueens(col + 1)) return true;

            board[i][col] = 0; // Backtrack
            steps++;
            logFile << "Paso " << steps << ": Retrocediendo - removiendo reina de (" << i + 1 << "," << col + 1 << ")\n\n";
            
            // Actualizar visualización después del backtrack
            window->clear(sf::Color::White);
            drawBoard();
            drawQueens();
            window->display();
            
            handleEvents();
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    return false;
}

void NQueens::run() {
    while (window->isOpen()) {
        handleEvents();
        window->clear(sf::Color::White);
        drawBoard();
        drawQueens();
        window->display();
    }
}