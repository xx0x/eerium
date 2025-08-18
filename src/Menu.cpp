#include "Menu.hpp"
#include <print>
#include <iostream>

using namespace daemonium;

Menu::Menu()
{
    options = {"Start Game", "Quit"};
}

int Menu::run()
{
    std::print("==== Daemonium ====\n");
    for (size_t i = 0; i < options.size(); ++i)
    {
        std::print("{}: {}\n", i + 1, options[i]);
    }

    int choice = 0;
    while (choice < 1 || choice > static_cast<int>(options.size()))
    {
        std::print("Choose option: ");
        std::cin >> choice;
    }

    return choice;
}
