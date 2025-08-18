#pragma once
#include <SDL.h>
#include <memory>

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool running = false;
};
