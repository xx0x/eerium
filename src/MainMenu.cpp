#include "MainMenu.hpp"

#include <print>

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

    new_options_.AddElement(std::make_unique<ui::ClickableText>("New Game", [this]()
                                                             {
        selected_option_ = 0;
        action_selected_ = true; }));

    new_options_.AddElement(std::make_unique<ui::ClickableText>("Help", [this]()
                                                             {
        selected_option_ = 1;
        action_selected_ = true; }));

    new_options_.AddElement(std::make_unique<ui::ClickableText>("Exit", [this]()
                                                             {
        selected_option_ = 2;
        action_selected_ = true; }));
}

void MainMenu::Reset()
{
    selected_option_ = 0;
    action_selected_ = false;
}

void MainMenu::HandleEvent(const SDL_Event& event)
{
    new_options_.HandleEvent(event);
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
    renderer.RenderText("EERIUM", window.width / 2, 100, sdl::kColorRed, *title_font_, sdl::Renderer::TextAlign::CENTER);

    // New Main Menu options
    new_options_.SetPosition(window.width / 2, window.height / 2);
    new_options_.Render(renderer);

    // Instructions
    renderer.RenderText("Use arrow keys to navigate, Enter to select",
                        window.width / 2, window.height - 80, sdl::kColorDarkGrey, *menu_font_, sdl::Renderer::TextAlign::CENTER);
}

MainMenu::Item MainMenu::GetActivatedItem() const
{
    if (!action_selected_)
    {
        return Item{};
    }

    return options_[selected_option_];
}
