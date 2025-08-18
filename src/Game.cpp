#include "Game.hpp"
#include <print>

using namespace daemonium;

Game::Game() = default;

Game::~Game()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

bool Game::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::print(stderr, "SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(kGameTitle, 800, 600, 0);
    if (!window)
    {
        std::print(stderr, "Window could not be created! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::print(stderr, "Renderer could not be created! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    running = true;
    return true;
}

void Game::run()
{
    SDL_Event e;
    int x = 100, y = 100;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                switch (e.key.key)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_UP:
                    y -= 10;
                    break;
                case SDLK_DOWN:
                    y += 10;
                    break;
                case SDLK_LEFT:
                    x -= 10;
                    break;
                case SDLK_RIGHT:
                    x += 10;
                    break;
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw a red rectangle (player)
        SDL_FRect rect = {(float)x, (float)y, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }
}
