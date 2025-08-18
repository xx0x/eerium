#include "Menu.hpp"
#include "Game.hpp"

using namespace eerium;

int main()
{
    Menu menu;
    int choice = menu.Run();
    if (choice == 1)
    {
        Game game;
        if (game.Init())
        {
            game.Run();
        }
    }
    return 0;
}
