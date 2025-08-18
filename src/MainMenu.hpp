#pragma once
#include <string>
#include <vector>
#include <SDL3/SDL.h>

namespace eerium
{
    

    class MainMenu
    {

    public:

        enum class Action
        {
            NONE,
            START_GAME,
            QUIT
        };

        MainMenu();

        void HandleEvent(const SDL_Event& event);
        void Render(SDL_Renderer* renderer);
        Action GetSelectedAction() const;

    private:
        std::vector<std::string> options_;
        int selected_option_ = 0;
        bool action_selected_ = false;
    };

}