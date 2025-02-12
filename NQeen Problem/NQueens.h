#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <fstream>

class NQueens {
private:
    int n;
    std::vector<std::vector<int>> board;
    std::ofstream logFile;
    int steps;
    int maxSteps;
    
    // Elementos gráficos
    sf::RenderWindow* window;
    sf::Texture queenTexture;
    sf::Sprite queenSprite;
    sf::RectangleShape cellShape;
    
    // Dimensiones de la ventana y celdas
    const int CELL_SIZE = 80;
    const int WINDOW_PADDING = 50;
    
    bool isSafe(int row, int col);
    void printBoardToFile(int row, int col);
    void clearBoard();
    void drawBoard();
    void drawQueens();
    void handleEvents();

public:
    NQueens(int size);
    ~NQueens();
    bool solveNQueens(int col);
    void run();
    int getSteps() const { return steps; }
    int getMaxSteps() const { return maxSteps; }
};
