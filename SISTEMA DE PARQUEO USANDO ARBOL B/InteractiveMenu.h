#pragma once
#include <vector>
#include <string>

class InteractiveMenu {
private:
    std::vector<std::string> options;
    int selectedOption;
    std::string title;

public:
    InteractiveMenu(const std::vector<std::string>& menuOptions, const std::string& menuTitle);
    int show();
    static int showSubMenu(const std::vector<std::string>& subOptions, const std::string& subMenuTitle);
};
