#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>

#include "MainMenu.hpp"
#include "Player.hpp"
#include "sdl/Context.hpp"
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
        QUIT
    };

    Game();

    void Run();

private:
    void HandleEvents();
    void Update();
    void Render();

    void PlayerMove(float delta_x, float delta_y);

    // RAII SDL resources - order matters for destruction
    sdl::Context context_;
    sdl::Window window_;
    sdl::Renderer renderer_;

    State current_state_ = State::MENU;

    // Game objects
    MainMenu menu_;

    // Game data
    Player player1_ = {"Hannah", sdl::kColorMagenta};

    static constexpr char kGameTitle[] = "Eerium";
};

}  // namespace eerium