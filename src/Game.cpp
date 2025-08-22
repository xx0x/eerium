#include "Game.hpp"

#include <print>
using namespace eerium;

//  | SDL_WINDOW_HIGH_PIXEL_DENSITY
// SDL_WINDOW_FULLSCREEN

Game::Game() : context_(SDL_INIT_VIDEO),
               window_(kGameTitle, 800, 600, SDL_WINDOW_RESIZABLE),
               renderer_(window_.Get(), nullptr)
{
    current_state_ = State::MENU;
    last_update_time_ = SDL_GetPerformanceCounter();
    last_render_time_ = last_update_time_;
    std::println("Game initialized successfully");
}

void Game::Run()
{
    while (current_state_ != State::QUIT)
    {
        Uint64 current_time = SDL_GetPerformanceCounter();
        Uint64 frequency = SDL_GetPerformanceFrequency();
        
        // Calculate delta time since last frame
        double delta_time = static_cast<double>(current_time - last_update_time_) / frequency;
        last_update_time_ = current_time;
        
        // Add delta time to accumulator
        update_accumulator_ += delta_time;
        
        // Always handle events
        HandleEvents();
        
        // Fixed timestep updates - run multiple updates if we've fallen behind
        while (update_accumulator_ >= kUpdateIntervalSeconds)
        {
            Update();
            update_accumulator_ -= kUpdateIntervalSeconds;
        }
        
        // Frame-limited rendering
        double time_since_last_render = static_cast<double>(current_time - last_render_time_) / frequency;
        if (time_since_last_render >= kRenderIntervalSeconds)
        {
            Render();
            last_render_time_ = current_time;
        }
        
        // Small sleep to prevent busy waiting and reduce CPU usage
        SDL_Delay(1);
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

            case State::HELP:
                if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                {
                    current_state_ = State::MENU;
                }
                break;
            case State::PLAYING:
                if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                {
                    current_state_ = State::MENU;
                }
                iso_grid_.HandleEvent(e);
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
            MainMenu::Item action = menu_.GetActivatedItem();
            if (!action.name.empty())
            {
                menu_.Reset();
                if (action.name == "start")
                {
                    iso_grid_.Reset();
                    current_state_ = State::PLAYING;
                    return;
                }
                else if (action.name == "help")
                {
                    current_state_ = State::HELP;
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
            // Update with fixed timestep delta time
            iso_grid_.Update();
            break;
        case State::HELP:
            break;
        case State::QUIT:
            break;
    }
}

void Game::StartGame()
{
    iso_grid_.Reset();
}

void Game::Render()
{
    switch (current_state_)
    {
        case State::MENU:
            menu_.Render(renderer_);
            break;
        case State::HELP:
            renderer_.Clear();
            break;
        case State::PLAYING:
            iso_grid_.Render(renderer_);
            break;
        case State::QUIT:
            break;
    }

    // Render FPS counter on top of everything in all states
    fps_counter_.Render(renderer_);

    SDL_RenderPresent(renderer_.Get());
}
