#pragma once

#include <string>

#include "objects/BaseObject.hpp"
#include "sdl/Color.hpp"

namespace eerium::objects
{

class Player : public BaseObject
{
public:
    Player(const std::string& name, sdl::Color color)
        : name_(name), color_(color)
    {
        SetSize(32, 64);
    }

    void Render(sdl::Renderer& renderer)
    {
        SDL_FRect rect = {GetPosition().x - GetSize().width / 2,
                          GetPosition().y - GetSize().height / 2,
                          GetSize().width,
                          GetSize().height};
        SDL_SetRenderDrawColor(renderer.Get(), color_.r, color_.g, color_.b, color_.a);
        SDL_RenderFillRect(renderer.Get(), &rect);
    }

private:
    std::string name_;
    sdl::Color color_;
};
}  // namespace eerium::objects