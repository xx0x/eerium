#include "Menu.hpp"
#include <print>
#include <iostream>

using namespace daemonium;

Menu::Menu()
{
    options_ = {"Start Game", "Quit"};
}

int Menu::Run()
{
    std::print("==== Daemonium ====\n");
    for (size_t i = 0; i < options_.size(); ++i)
    {
        std::print("{}: {}\n", i + 1, options_[i]);
    }

    int choice = 0;
    while (choice < 1 || choice > static_cast<int>(options_.size()))
    {
        std::print("Choose option: ");
        std::cin >> choice;
    }

    return choice;
}
