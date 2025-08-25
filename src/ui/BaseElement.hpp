#pragma once

#include <functional>

#include "sdl/Renderer.hpp"

namespace eerium::ui
{

class BaseElement
{
protected:
    typedef std::function<void()> ClickCallback;
    typedef std::function<void(bool)> HoverCallback;

public:
    virtual ~BaseElement() = default;
    virtual void Render(sdl::Renderer& renderer) = 0;
    void SetPosition(float x, float y)
    {
        position_x_ = x;
        position_y_ = y;
    }

    void SetClickCallback(ClickCallback callback)
    {
        on_click_ = callback;
    }

    void SetHoverCallback(HoverCallback callback)
    {
        on_hover_ = callback;
    }

    void HandleEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            bool prev_hovered = is_hovered_;
            // Check if the mouse is over the element
            is_hovered_ = (event.motion.x >= position_x_ && event.motion.x <= position_x_ + width_ &&
                           event.motion.y >= position_y_ && event.motion.y <= position_y_ + height_);
            if (prev_hovered != is_hovered_ && on_hover_)
            {
                on_hover_(is_hovered_);
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && is_hovered_)
        {
            // Call the click callback if the element is clicked
            if (on_click_)
            {
                on_click_();
            }
        }
    }

    void TriggerOnClick()
    {
        if (on_click_)
        {
            on_click_();
        }
    }

    void TriggerOnHover(bool hovered)
    {
        is_hovered_ = hovered;
    }

    float GetWidth() const
    {
        return width_;
    }

    float GetHeight() const
    {
        return height_;
    }

protected:
    float position_x_ = 0.0f;
    float position_y_ = 0.0f;
    float width_ = 0.0f;
    float height_ = 0.0f;

    ClickCallback on_click_ = nullptr;
    HoverCallback on_hover_ = nullptr;

    bool is_hovered_ = false;
};

}  // namespace eerium::ui