#include "MainMenu.hpp"

#include <print>

using namespace eerium;

MainMenu::MainMenu() { options_ = {"Start Game", "Quit"}; }

void MainMenu::HandleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN)
    {
        switch (event.key.key)
        {
            case SDLK_UP:
                selected_option_ = (selected_option_ - 1 + options_.size()) % options_.size();
                break;
            case SDLK_DOWN:
                selected_option_ = (selected_option_ + 1) % options_.size();
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                action_selected_ = true;
                break;
        }
    }
}

void MainMenu::Render(SDL_Renderer* renderer)
{
    // Clear screen with dark blue background
    SDL_SetRenderDrawColor(renderer, 20, 30, 60, 255);
    SDL_RenderClear(renderer);

    // Get window size for centering
    SDL_Window* window = SDL_GetRenderWindow(renderer);
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    // Title area
    SDL_FRect title_rect = {static_cast<float>(window_width) / 2.0f - 100, 100,
                            200, 60};
    SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
    SDL_RenderFillRect(renderer, &title_rect);

    // MainMenu options
    float option_y = 200;
    for (size_t i = 0; i < options_.size(); ++i)
    {
        SDL_FRect option_rect = {static_cast<float>(window_width) / 2.0f - 80,
                                 option_y + static_cast<float>(i) * 60, 160,
                                 40};

        // Highlight selected option
        if (i == selected_option_)
        {
            SDL_SetRenderDrawColor(renderer, 200, 200, 100, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        }

        SDL_RenderFillRect(renderer, &option_rect);

        // Draw border
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderRect(renderer, &option_rect);
    }

    // Instructions
    SDL_FRect instruction_rect = {static_cast<float>(window_width) / 2.0f - 120,
                                  static_cast<float>(window_height) - 80, 240,
                                  30};
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderFillRect(renderer, &instruction_rect);
}

MainMenu::Action MainMenu::GetSelectedAction() const
{
    if (!action_selected_)
    {
        return Action::NONE;
    }

    switch (selected_option_)
    {
        case 0:
            return Action::START_GAME;
        case 1:
            return Action::QUIT;
        default:
            return Action::NONE;
    }
}
