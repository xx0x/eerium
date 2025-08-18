#include "Game.hpp"

#include <print>

using namespace eerium;

Game::Game()
    : context_(SDL_INIT_VIDEO), window_(kGameTitle, 800, 600, 0), renderer_(window_.Get(), nullptr)
{
    // Initialize resource manager and load default font
    sdl::ResourceManager::Instance().Initialize();
    sdl::ResourceManager::Instance().LoadFont("default",
                                              "../resources/fonts/UncialAntiqua-Regular.ttf", 24);

    current_state_ = State::MENU;
    std::print("Game initialized successfully\n");
}

void Game::Run()
{
    while (current_state_ != State::QUIT)
    {
        HandleEvents();
        Update();
        Render();
    }

    // ResourceManager will automatically clean up when it goes out of scope
    sdl::ResourceManager::Instance().Shutdown();
}

void Game::HandleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
        {
            current_state_ = State::QUIT;
            return;
        }

        switch (current_state_)
        {
            case State::MENU:
                if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                {
                    current_state_ = State::QUIT;
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
            break;
    }
}

void Game::Render()
{
    switch (current_state_)
    {
        case State::MENU:
            menu_.Render(renderer_.Get());
            break;
        case State::PLAYING:
        {
            // Clear screen
            SDL_SetRenderDrawColor(renderer_.Get(), 0, 0, 0, 255);
            SDL_RenderClear(renderer_.Get());

            // Draw a red rectangle (player)
            SDL_FRect rect = {(float)player_x_, (float)player_y_, 50.0f, 50.0f};
            SDL_SetRenderDrawColor(renderer_.Get(), 200, 0, 0, 255);
            SDL_RenderFillRect(renderer_.Get(), &rect);
        }
        break;
        case State::QUIT:
            break;
    }

    SDL_RenderPresent(renderer_.Get());
}
