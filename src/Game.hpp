#pragma once
#include <SDL3/SDL.h>
#include <memory>

namespace daemonium
{
    class Game
    {
    public:
        Game();
        ~Game();

        bool Init();
        void Run();

    private:
        SDL_Window *window_ = nullptr;
        SDL_Renderer *renderer_ = nullptr;
        bool running_ = false;
        static constexpr char kGameTitle[] = "Daemonium";
    };

}