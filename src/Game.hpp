#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>

#include "MainMenu.hpp"

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
    ~Game();

    bool Init();
    void Run();

private:
    void HandleEvents();
    void Update();
    void Render();

    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;
    bool running_ = false;
    State current_state_ = State::MENU;

    // Game objects
    MainMenu menu_;

    // Game data
    int player_x_ = 100;
    int player_y_ = 100;

    static constexpr char kGameTitle[] = "Eerium";
};

}  // namespace eerium