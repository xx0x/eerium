#include "Game.hpp"
#include <print>

using namespace daemonium;

Game::Game() = default;

Game::~Game()
{
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
    }
    SDL_Quit();
}

bool Game::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::print(stderr, "SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(kGameTitle, 800, 600, 0);
    if (!window_)
    {
        std::print(stderr, "Window could not be created! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_)
    {
        std::print(stderr, "Renderer could not be created! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    running_ = true;
    return true;
}

void Game::Run()
{
    SDL_Event e;
    int x = 100, y = 100;

    while (running_)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                running_ = false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                switch (e.key.key)
                {
                case SDLK_ESCAPE:
                    running_ = false;
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
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);

        // Draw a red rectangle (player)
        SDL_FRect rect = {(float)x, (float)y, 50.0f, 50.0f};
        SDL_SetRenderDrawColor(renderer_, 200, 0, 0, 255);
        SDL_RenderFillRect(renderer_, &rect);

        SDL_RenderPresent(renderer_);
    }
}
