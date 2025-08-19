#pragma once

#include <string>

#include "sdl/Color.hpp"

namespace eerium
{

class Player
{
public:
    struct Position
    {
        float x = 0.0f;
        float y = 0.0f;
    };

    Player(const std::string& name, sdl::Color color)
        : name_(name), color_(color) {}

    void SetPosition(Position position)
    {
        position_ = position;
    }

    void SetPosition(float x, float y)
    {
        position_.x = x;
        position_.y = y;
    }

    Position GetPosition() const
    {
        return position_;
    }

    void Render(sdl::Renderer& renderer) const
    {
        SDL_FRect rect = {position_.x - sprite_width_ / 2, position_.y - sprite_height_ / 2, sprite_width_, sprite_height_};
        SDL_SetRenderDrawColor(renderer.Get(), color_.r, color_.g, color_.b, color_.a);
        SDL_RenderFillRect(renderer.Get(), &rect);
    }

private:
    std::string name_;
    sdl::Color color_;
    Position position_;

    float sprite_width_ = 32.0f;
    float sprite_height_ = 64.0f;
};
}  // namespace eerium