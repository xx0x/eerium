#include "Menu.hpp"
#include <iostream>

Menu::Menu() {
    options = {"Start Game", "Quit"};
}

int Menu::run() {
    std::cout << "==== Daemonium ====\n";
    for (size_t i = 0; i < options.size(); ++i) {
        std::cout << i + 1 << ". " << options[i] << "\n";
    }

    int choice = 0;
    while (choice < 1 || choice > static_cast<int>(options.size())) {
        std::cout << "Choose option: ";
        std::cin >> choice;
    }

    return choice;
}
