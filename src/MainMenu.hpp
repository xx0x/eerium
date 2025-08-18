#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <array>
#include <string>
#include <vector>

namespace eerium
{

class MainMenu
{
public:
    enum class Action
    {
        NONE,
        START_GAME,
        HELP,
        QUIT
    };

    MainMenu();

    bool Init(); // Add initialization method
    void DeInit(); // Explicit shutdown to release SDL_ttf resources before global quit
    void Reset();
    void HandleEvent(const SDL_Event& event);
    void Render(SDL_Renderer* renderer);
    Action GetSelectedAction() const;

private:
    std::array<std::string, 3> options_ = {
        "Start Game",
        "Help",
        "Quit"};
    int selected_option_ = 0;
    bool action_selected_ = false;
    TTF_Font* font_ = nullptr;
    
    void RenderText(SDL_Renderer* renderer, const std::string& text, int x, int y, SDL_Color color);
public:
    ~MainMenu();
};

}  // namespace eerium