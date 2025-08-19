#pragma once

#include "BaseObject.hpp"

namespace eerium::objects
{

class Rock : public BaseObject
{
public:
    Rock()
    {
        SetSize(40, 40);
    }

    void Render(sdl::Renderer& renderer) override
    {
        SDL_FRect rock_body = {GetPosition().x - GetSize().width / 2,
                               GetPosition().y - GetSize().height / 2,
                               GetSize().width,
                               GetSize().height};
        SDL_SetRenderDrawColor(renderer.Get(), 80, 80, 80, 255);  // Dark gray
        SDL_RenderFillRect(renderer.Get(), &rock_body);

        // RenderBoundingBox(renderer);
    }
};

}  // namespace eerium::objects