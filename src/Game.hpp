#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>

#include "MainMenu.hpp"
#include "objects/Player.hpp"
#include "objects/Tree.hpp"
#include "objects/Rock.hpp"
#include "sdl/Context.hpp"
#include "sdl/Renderer.hpp"
#include "sdl/ResourceManager.hpp"
#include "sdl/Window.hpp"
#include "sdl/FpsCounter.hpp"
#include "Level.hpp"

namespace eerium
{

class Game
{
public:
    enum class State
    {
        MENU,
        PLAYING,
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

    // Level
    std::unique_ptr<Level> level_;

    static constexpr char kGameTitle[] = "Eerium";
};

}  // namespace eerium