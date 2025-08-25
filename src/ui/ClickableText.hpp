#pragma once

#include <string>

#include "sdl/Color.hpp"
#include "sdl/ResourceManager.hpp"
#include "ui/BaseElement.hpp"

namespace eerium::ui
{

class ClickableText : public BaseElement
{
public:
    ClickableText(const std::string& text, ClickCallback callback)
        : text_(text)
    {
        // fake width/height
        width_ = 200;
        height_ = 40;

        on_click_ = callback;
        SetColors(sdl::kColorLightGrey, sdl::kColorYellow);
    }

    void SetColors(const sdl::Color& color, const sdl::Color& hover_color)
    {
        color_ = color;
        hover_color_ = hover_color;
    }

    void Render(sdl::Renderer& renderer) override
    {
        auto font = sdl::ResourceManager::Instance().GetDefaultFont();
        if (!font)
        {
            return;
        }
        // render background
        SDL_SetRenderDrawColor(renderer.Get(), color_.r, color_.g, color_.b, 20);
        SDL_FRect rect = {position_x_, position_y_, width_, height_};
        SDL_RenderFillRect(renderer.Get(), &rect);

        // render text
        renderer.RenderText(text_,
                            position_x_,
                            position_y_,
                            is_hovered_ ? hover_color_ : color_,
                            font.value());
    }

private:
    std::string text_;
    sdl::Color color_;
    sdl::Color hover_color_;
};

}  // namespace eerium::ui