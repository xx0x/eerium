#pragma once

#include "BaseObject.hpp"

namespace eerium::objects
{

class Tree : public BaseObject
{
public:
    Tree()
    {
        SetSize(60, 100);
    }

    void Render(sdl::Renderer& renderer) override
    {
        SDL_FRect treetop = {GetPosition().x - GetSize().width / 2,
                             GetPosition().y - GetSize().height / 2,
                             GetSize().width,
                             GetSize().width};
        SDL_SetRenderDrawColor(renderer.Get(), 34, 139, 34, 255);  // Forest green
        SDL_RenderFillRect(renderer.Get(), &treetop);

        float trunk_height = GetSize().height - GetSize().width;

        SDL_FRect trunk = {GetPosition().x - GetSize().width / 8,
                           GetPosition().y - GetSize().height / 2 + GetSize().width,
                           GetSize().width / 4,
                           trunk_height};
        SDL_SetRenderDrawColor(renderer.Get(), 139, 69, 19, 255);  // SaddleBrown
        SDL_RenderFillRect(renderer.Get(), &trunk);

        // RenderBoundingBox(renderer);
    }
};

}  // namespace eerium::objects