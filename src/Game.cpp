#include "Game.hpp"

#include <print>

using namespace eerium;

Game::Game()
    : context_(SDL_INIT_VIDEO), window_(kGameTitle, 800, 600, 0), renderer_(window_.Get(), nullptr)
{
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
                            PlayerMove(0, -10);
                            break;
                        case SDLK_DOWN:
                            PlayerMove(0, 10);
                            break;
                        case SDLK_LEFT:
                            PlayerMove(-10, 0);
                            break;
                        case SDLK_RIGHT:
                            PlayerMove(10, 0);
                            break;
                    }
                }
                break;
            case State::QUIT:
                break;
        }
    }
}

void Game::PlayerMove(float delta_x, float delta_y)
{
    player1_.SetPosition(player1_.GetPosition().x + delta_x, player1_.GetPosition().y + delta_y);
}

void Game::Update()
{
    switch (current_state_)
    {
        case State::MENU:
        {
            MainMenu::Item action = menu_.GetActivatedItem();
            if (!action.name.empty())
            {
                menu_.Reset();
                if (action.name == "start")
                {
                    current_state_ = State::PLAYING;
                    return;
                }
                else if (action.name == "quit")
                {
                    current_state_ = State::QUIT;
                    return;
                }
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
            menu_.Render(renderer_);
            break;
        case State::PLAYING:
        {
            // Clear screen
            renderer_.Clear(sdl::kColorBlack);

            // Draw the player
            player1_.Render(renderer_);
        }
        break;
        case State::QUIT:
            break;
    }

    SDL_RenderPresent(renderer_.Get());
}
