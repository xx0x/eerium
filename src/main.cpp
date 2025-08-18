#include "Game.hpp"

using namespace eerium;

int main()
{
    Game game;
    if (game.Init())
    {
        game.Run();
    }
    return 0;
}
