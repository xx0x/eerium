#include "Game.hpp"

#include <print>

using namespace eerium;

Game::Game() = default;

Game::~Game()
{
    // Release menu (font) resources before quitting TTF
    menu_.DeInit();
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
    }
    if (window_)
    {
        SDL_DestroyWindow(window_);
    }
    TTF_Quit();
    SDL_Quit();
}

bool Game::Init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::print(stderr, "SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    if (!TTF_Init())
    {
        std::print(stderr, "SDL_ttf could not initialize! SDL_Error: {}\n", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(kGameTitle, 800, 600, 0);
    if (!window_)
    {
        std::print(stderr, "Window could not be created! SDL_Error: {}\n",
                   SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    if (!renderer_)
    {
        std::print(stderr, "Renderer could not be created! SDL_Error: {}\n",
                   SDL_GetError());
        return false;
    }
    
    // Initialize menu after SDL_ttf is ready
    if (!menu_.Init())
    {
        std::print(stderr, "Menu could not initialize!\n");
        return false;
    }

    running_ = true;
    return true;
}

void Game::Run()
{
    while (running_)
    {
        HandleEvents();
        Update();
        Render();
    }
}

void Game::HandleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            running_ = false;
            return;
        }

        switch (current_state_)
        {
            case State::MENU:
                if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                {
                    running_ = false;
                    return;
                }
                menu_.HandleEvent(e);
                break;
            case State::PLAYING:
                if (e.type == SDL_EVENT_KEY_DOWN)
                {
                    switch (e.key.key)
                    {
                        case SDLK_ESCAPE:
                            current_state_ = State::MENU;
                            break;
                        case SDLK_UP:
                            player_y_ -= 10;
                            break;
                        case SDLK_DOWN:
                            player_y_ += 10;
                            break;
                        case SDLK_LEFT:
                            player_x_ -= 10;
                            break;
                        case SDLK_RIGHT:
                            player_x_ += 10;
                            break;
                    }
                }
                break;
            case State::QUIT:
                running_ = false;
                break;
        }
    }
}

void Game::Update()
{
    switch (current_state_)
    {
        case State::MENU:
        {
            MainMenu::Action action = menu_.GetSelectedAction();
            switch (action)
            {
                case MainMenu::Action::START_GAME:
                    current_state_ = State::PLAYING;
                    // Reset menu for next time
                    menu_.Reset();
                    break;
                case MainMenu::Action::HELP:
                    // TODO: Implement help screen
                    break;
                case MainMenu::Action::QUIT:
                    current_state_ = State::QUIT;
                    break;
                case MainMenu::Action::NONE:
                    // Stay in menu
                    break;
            }
        }
        break;
        case State::PLAYING:
            // Game logic would go here
            break;
        case State::QUIT:
            running_ = false;
            break;
    }
}

void Game::Render()
{
    switch (current_state_)
    {
        case State::MENU:
            menu_.Render(renderer_);
            break;
        case State::PLAYING:
        {
            // Clear screen
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderClear(renderer_);

            // Draw a red rectangle (player)
            SDL_FRect rect = {(float)player_x_, (float)player_y_, 50.0f, 50.0f};
            SDL_SetRenderDrawColor(renderer_, 200, 0, 0, 255);
            SDL_RenderFillRect(renderer_, &rect);
        }
        break;
        case State::QUIT:
            break;
    }

    SDL_RenderPresent(renderer_);
}
