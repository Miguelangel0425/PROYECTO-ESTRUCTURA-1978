#include "InteractiveMenu.h"
#include <iostream>
#include <conio.h>
#include <windows.h>

InteractiveMenu::InteractiveMenu(const std::vector<std::string>& menuOptions, const std::string& menuTitle) 
    : options(menuOptions), selectedOption(0), title(menuTitle) {}

int InteractiveMenu::show() {
    while (true) {
        system("cls");

        std::cout << "\033[1;32m";
        std::cout << "========================================" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "\033[0m"; 
        std::cout << "        " << title << std::endl;
        std::cout << "\033[1;32m"; 
        std::cout << "========================================" << std::endl;
        std::cout << "========================================" << std::endl;

        std::cout << "\033[0m";
        for (size_t i = 0; i < options.size(); i++) {
            if (i == selectedOption) {
                std::cout << " > " << options[i] << " < " << std::endl;
            } else {
                std::cout << "   " << options[i] << std::endl;
            }
        }

        std::cout << "\033[1;32m";
        std::cout << "========================================" << std::endl;
        std::cout << "\033[0m"; 

        char key = _getch();

        if (key == 'w' || key == 72) { // 'w' or arrow up
            selectedOption = (selectedOption - 1 + options.size()) % options.size();
        } else if (key == 's' || key == 80) { // 's' or arrow down
            selectedOption = (selectedOption + 1) % options.size();
        } else if (key == 13) { // Enter key
            return selectedOption;
        }
    }
}

int InteractiveMenu::showSubMenu(const std::vector<std::string>& subOptions, const std::string& subMenuTitle) {
    InteractiveMenu subMenu(subOptions, subMenuTitle);
    return subMenu.show();
}
