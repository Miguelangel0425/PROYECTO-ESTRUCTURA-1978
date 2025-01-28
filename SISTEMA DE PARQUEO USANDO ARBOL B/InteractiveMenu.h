#pragma once
#include <vector>
#include <string>
#include <functional>
#include <conio.h> // Para _getch()

class InteractiveMenu {
private:
    std::vector<std::string> options;
    int selectedOption;
    
public:
    InteractiveMenu(const std::vector<std::string>& menuOptions);
    int show();
    static int showSubMenu(const std::vector<std::string>& subOptions);
};
