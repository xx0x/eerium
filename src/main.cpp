#include <exception>
#include <print>

#include "Game.hpp"

using namespace eerium;

int main()
{
    try
    {
        Game game;
        game.Run();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Error: {}", e.what());
        return 1;
    }
}
