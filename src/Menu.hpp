#pragma once
#include <string>
#include <vector>

namespace eerium
{

    class Menu
    {
    public:
        Menu();

        // Show the menu and return the chosen option
        int Run();

    private:
        std::vector<std::string> options_;
    };

}