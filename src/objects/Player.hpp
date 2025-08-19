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

    void Render(sdl::Renderer& renderer) override
    {
        SDL_FRect rect = {GetPosition().x - GetSize().width / 2,
                          GetPosition().y - GetSize().height / 2,
                          GetSize().width,
                          GetSize().height};
        SDL_SetRenderDrawColor(renderer.Get(), color_.r, color_.g, color_.b, color_.a);
        SDL_RenderFillRect(renderer.Get(), &rect);
    }

    void WalkTo(Position position)
    {
        target_position_ = position;
    }

    void Update() override
    {
        // Implement simple linear movement towards the target position
        Position current_position = GetPosition();
        if (current_position.x < target_position_.x)
            current_position.x += 1.0f;
        else if (current_position.x > target_position_.x)
            current_position.x -= 1.0f;

        if (current_position.y < target_position_.y)
            current_position.y += 1.0f;
        else if (current_position.y > target_position_.y)
            current_position.y -= 1.0f;

        SetPosition(current_position);
    }

private:
    std::string name_;
    sdl::Color color_;
    Position target_position_;
};
}  // namespace eerium::objects