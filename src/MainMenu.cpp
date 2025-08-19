#include "MainMenu.hpp"

#include <print>

#include "Colors.hpp"
#include "sdl/ResourceManager.hpp"

using namespace eerium;

MainMenu::MainMenu()
{
    menu_font_ = sdl::ResourceManager::Instance().GetDefaultFont();
    title_font_ = sdl::ResourceManager::Instance().GetFont("title");
    if (!menu_font_ || !title_font_ || !menu_font_->IsValid() || !title_font_->IsValid())
    {
        std::println(stderr, "Failed to get the fonts");
    }
}

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

void MainMenu::Render(sdl::Renderer& renderer)
{
    renderer.Clear();

    // Check if font is available
    if (!menu_font_ || !title_font_)
    {
        std::println(stderr, "Fonts not loaded");
        return;
    }

    auto window = renderer.GetWindowSize();

    // Render title
    renderer.RenderText("EERIUM", window.width / 2, 100, kColorRedText, *title_font_, sdl::Renderer::TextAlign::CENTER);

    // MainMenu options
    float option_y = 200;
    for (size_t i = 0; i < options_.size(); ++i)
    {
        sdl::Color text_color;

        // Highlight selected option
        if (i == selected_option_)
        {
            text_color = kColorSelectedText;
        }
        else
        {
            text_color = kColorNormalText;
        }

        // Center the text horizontally
        float text_x = window.width / 2;
        float text_y = option_y + i * 60;

        renderer.RenderText(options_[i].label, text_x, text_y, text_color, *menu_font_, sdl::Renderer::TextAlign::CENTER);
    }

    // Instructions
    renderer.RenderText("Use arrow keys to navigate, Enter to select",
                        window.width / 2, window.height - 80, kColorNoteText, *menu_font_, sdl::Renderer::TextAlign::CENTER);
}

MainMenu::Item MainMenu::GetActivatedItem() const
{
    if (!action_selected_)
    {
        return Item{};
    }

    return options_[selected_option_];
}
