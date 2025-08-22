#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>

#include "IsoGrid.hpp"
#include "MainMenu.hpp"
#include "sdl/Context.hpp"
#include "sdl/FpsCounter.hpp"
#include "sdl/Renderer.hpp"
#include "sdl/ResourceManager.hpp"
#include "sdl/Window.hpp"

namespace eerium
{

class Game
{
public:
    enum class State
    {
        MENU,
        PLAYING,
        HELP,
        QUIT
    };

    Game();

    void Run();

private:
    void HandleEvents();
    void Update();
    void Render();

    void StartGame();

    // RAII SDL resources - order matters for destruction
    sdl::Context context_;
    sdl::Window window_;
    sdl::Renderer renderer_;

    State current_state_ = State::MENU;

    // UI
    MainMenu menu_;
    sdl::FpsCounter fps_counter_;

    // Playground
    IsoGrid iso_grid_;

    static constexpr char kGameTitle[] = "Eerium";
};

}  // namespace eerium