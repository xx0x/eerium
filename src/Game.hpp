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

        bool init();
        void run();

    private:
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        bool running = false;
        static constexpr char kGameTitle[] = "Daemonium";
    };

}