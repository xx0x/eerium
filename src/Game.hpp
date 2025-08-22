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

    // Timing constants
    static constexpr char kGameTitle[] = "Eerium";
    static constexpr double kUpdateIntervalSeconds = 1.0 / 50.0;  // 50 updates per second (20ms)
    static constexpr double kTargetRenderFps = 120.0;
    static constexpr double kRenderIntervalSeconds = 1.0 / kTargetRenderFps;
    
    // Timing variables
    Uint64 last_update_time_ = 0;
    Uint64 last_render_time_ = 0;
    double update_accumulator_ = 0.0;
};

}  // namespace eerium