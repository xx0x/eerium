#pragma once
#include <string>
#include <vector>

class Menu {
public:
    Menu();

    // Show the menu and return the chosen option
    int run();

private:
    std::vector<std::string> options;
};
