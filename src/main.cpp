#include "Game.hpp"
#include <print>
#include <exception>

using namespace eerium;

int main()
{
    try {
        Game game;
        game.Run();
        return 0;
    }
    catch (const std::exception& e) {
        std::print(stderr, "Error: {}\n", e.what());
        return 1;
    }
}
