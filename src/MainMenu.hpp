#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <string>
#include <vector>
#include <optional>

#include "sdl/Font.hpp"
#include "sdl/Renderer.hpp"

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
    void Render(sdl::Renderer& renderer);
    Item GetActivatedItem() const;

private:
    std::array<Item, 3> options_ = {
        Item{.name = "start", .label = "Start Game"},
        Item{.name = "help", .label = "Help"},
        Item{.name = "quit", .label = "Quit"}};

    int selected_option_ = 0;
    bool action_selected_ = false;
    std::optional<sdl::Font> font_;
};

}  // namespace eerium