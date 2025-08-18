#include "MainMenu.hpp"

#include <print>

#include "sdl/ResourceManager.hpp"

using namespace eerium;

void MainMenu::Reset()
{
    selected_option_ = 0;
    action_selected_ = false;
}

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

void MainMenu::RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color)
{
    const auto& font = sdl::ResourceManager::Instance().GetDefaultFont();
    if (!font.IsValid())
        return;

    SDL_Surface* text_surface = TTF_RenderText_Solid(font.Get(), text.c_str(), text.length(), color);
    if (!text_surface)
    {
        std::print(stderr, "Unable to render text surface! SDL_Error: {}\n", SDL_GetError());
        return;
    }

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture)
    {
        std::print(stderr, "Unable to create texture from rendered text! SDL_Error: {}\n", SDL_GetError());
        SDL_DestroySurface(text_surface);
        return;
    }

    int text_width = text_surface->w;
    int text_height = text_surface->h;
    SDL_DestroySurface(text_surface);

    // Center the text at the given x position
    SDL_FRect render_quad = {static_cast<float>(x - text_width / 2), static_cast<float>(y), static_cast<float>(text_width), static_cast<float>(text_height)};
    SDL_RenderTexture(renderer, text_texture, nullptr, &render_quad);

    SDL_DestroyTexture(text_texture);
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

    // Render title
    SDL_Color title_color = {200, 200, 255, 255};  // Light blue
    RenderText(renderer, "EERIUM", window_width / 2, 100, title_color);

    // MainMenu options
    float option_y = 200;
    for (size_t i = 0; i < options_.size(); ++i)
    {
        SDL_Color text_color;

        // Highlight selected option
        if (i == selected_option_)
        {
            text_color = {255, 255, 100, 255};  // Yellow for selected
        }
        else
        {
            text_color = {200, 200, 200, 255};  // Light gray for unselected
        }

        // Center the text horizontally
        int text_x = window_width / 2;
        int text_y = static_cast<int>(option_y + i * 60);

        RenderText(renderer, options_[i], text_x, text_y, text_color);
    }

    // Instructions
    SDL_Color instruction_color = {150, 150, 150, 255};  // Gray
    RenderText(renderer, "Use arrow keys to navigate, Enter to select",
               window_width / 2, window_height - 80, instruction_color);
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
            return Action::HELP;
        case 2:
            return Action::QUIT;
        default:
            return Action::NONE;
    }
}
