#include "InteractiveMenu.h"
#include <iostream>
#include <windows.h>

InteractiveMenu::InteractiveMenu(const std::vector<std::string>& menuOptions) 
    : options(menuOptions), selectedOption(0) {}

int InteractiveMenu::show() {
    while (true) {
        system("cls");
        for (size_t i = 0; i < options.size(); i++) {
            if (i == selectedOption) {
                std::cout << "> " << options[i] << " <" << std::endl;
            } else {
                std::cout << "  " << options[i] << std::endl;
            }
        }

        int key = _getch();
        switch (key) {
            case 'w':
            case 72: // Flecha arriba
                selectedOption = (selectedOption - 1 + options.size()) % options.size();
                break;
            case 's':
            case 80: // Flecha abajo
                selectedOption = (selectedOption + 1) % options.size();
                break;
            case 13: // Enter
                return selectedOption;
        }
    }
}

int InteractiveMenu::showSubMenu(const std::vector<std::string>& subOptions) {
    InteractiveMenu subMenu(subOptions);
    return subMenu.show();
}