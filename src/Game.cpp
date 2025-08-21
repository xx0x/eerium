#include "Game.hpp"

#include <print>
using namespace eerium;

Game::Game()
    : context_(SDL_INIT_VIDEO), window_(kGameTitle, 800, 600, 0), renderer_(window_.Get(), nullptr)
{
    current_state_ = State::MENU;
    std::println("Game initialized successfully");
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
                if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                {
                    current_state_ = State::MENU;
                }
                if (level_ != nullptr)
                {
                    level_->HandleEvent(e);
                }
                break;
            case State::QUIT:
                break;
        }
    }
}

void Game::Update()
{
    // Update FPS counter
    fps_counter_.Update();

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
                    StartGame();
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
            if (level_ != nullptr)
            {
                level_->Update();
            }
            break;
        case State::QUIT:
            break;
    }
}

void Game::StartGame()
{
    level_ = std::make_unique<Level>();
}

void Game::Render()
{
    switch (current_state_)
    {
        case State::MENU:
            menu_.Render(renderer_);
            break;
        case State::PLAYING:
            if (level_ != nullptr)
            {
                level_->Render(renderer_);
            }
            break;
        case State::QUIT:
            break;
    }

    // Render FPS counter on top of everything in all states
    fps_counter_.Render(renderer_);

    SDL_RenderPresent(renderer_.Get());
}
