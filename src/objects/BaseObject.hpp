#pragma once

#include <format>
#include <string>

#include "sdl/Renderer.hpp"

namespace eerium::objects
{

class BaseObject
{
public:
    struct Position
    {
        float x = 0.0f;
        float y = 0.0f;
    };
    struct Size
    {
        float width = 0.0f;
        float height = 0.0f;
    };

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

    Size GetSize() const
    {
        return size_;
    }

    virtual void Render(sdl::Renderer& renderer) = 0;

    void RenderBoundingBox(sdl::Renderer& renderer)
    {
        SDL_FRect bounding_box = {GetPosition().x - GetSize().width / 2,
                                  GetPosition().y - GetSize().height / 2,
                                  GetSize().width,
                                  GetSize().height};
        SDL_SetRenderDrawColor(renderer.Get(), 255, 182, 193, 255);  // Light pink
        SDL_RenderRect(renderer.Get(), &bounding_box);
    }

protected:
    Position position_;
    Size size_;

    void SetSize(Size size)
    {
        size_ = size;
    }

    void SetSize(float width, float height)
    {
        size_.width = width;
        size_.height = height;
    }
};
}  // namespace eerium::objects

// Formatter specializations for std::println support
template <>
struct std::formatter<eerium::objects::BaseObject::Position>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const eerium::objects::BaseObject::Position& pos, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[x: {}, y: {}]", pos.x, pos.y);
    }
};

template <>
struct std::formatter<eerium::objects::BaseObject::Size>
{
    constexpr auto parse(std::format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(const eerium::objects::BaseObject::Size& size, std::format_context& ctx) const
    {
        return std::format_to(ctx.out(), "[width: {}, height: {}]", size.width, size.height);
    }
};