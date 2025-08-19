#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <string>
#include <vector>
#include <optional>

#include "sdl/Font.hpp"

namespace eerium
{

class MainMenu
{
public:
    struct Item
    {
        std::string name = "";
        std::string label = "";
    };

    MainMenu();
    void Reset();
    void HandleEvent(const SDL_Event& event);
    void Render(SDL_Renderer* renderer);
    Item GetActivatedItem() const;

private:
    std::array<Item, 3> options_ = {
        Item{.name = "start", .label = "Start Game"},
        Item{.name = "help", .label = "Help"},
        Item{.name = "quit", .label = "Quit"}};

    int selected_option_ = 0;
    bool action_selected_ = false;
    std::optional<sdl::Font> font_;

    void RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);};

}  // namespace eerium