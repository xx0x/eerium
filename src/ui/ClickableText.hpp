#pragma once

#include <string>
#include <string_view>

#include "sdl/Color.hpp"
#include "sdl/ResourceManager.hpp"
#include "ui/BaseElement.hpp"

namespace eerium::ui
{

class ClickableText : public BaseElement
{
public:
    struct ColorScheme
    {
        sdl::Color normal = sdl::kColorLightGrey;
        sdl::Color hovered = sdl::kColorYellow;
        sdl::Color focused = sdl::kColorYellow;
        sdl::Color pressed = sdl::kColorRed;
        sdl::Color disabled = sdl::kColorDarkGrey;
        sdl::Color background = sdl::kColorDarkGrey;
    };

    explicit ClickableText(std::string_view text, ClickHandler&& click_handler = nullptr)
        : text_(text)
    {
        if (click_handler)
        {
            SetClickHandler(std::move(click_handler));
        }

        // Calculate proper size based on text
        UpdateSize();
    }

    // Modern string handling
    void SetText(std::string_view text)
    {
        text_ = text;
        UpdateSize();
    }

    [[nodiscard]] const std::string& GetText() const noexcept { return text_; }

    void SetColorScheme(const ColorScheme& scheme) noexcept
    {
        colors_ = scheme;
    }

    void SetPadding(float horizontal, float vertical) noexcept
    {
        padding_.horizontal = horizontal;
        padding_.vertical = vertical;
        UpdateSize();
    }

    void Render(sdl::Renderer& renderer) override
    {
        auto font = sdl::ResourceManager::Instance().GetDefaultFont();
        if (!font) return;

        // Render background
        const auto bg_color = colors_.background;
        SDL_SetRenderDrawColor(renderer.Get(), bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_FRect rect = {GetX(), GetY(), GetWidth(), GetHeight()};
        SDL_RenderFillRect(renderer.Get(), &rect);

        // Choose text color based on state
        const sdl::Color text_color = GetCurrentTextColor();

        // Render text with padding
        renderer.RenderText(text_,
                            GetX() + padding_.horizontal,
                            GetY() + padding_.vertical,
                            text_color,
                            font.value());
    }

protected:
    void OnStateChanged(const ElementState& old_state, const ElementState& new_state) override
    {
        // Could add animation triggers here, sound effects, etc.
        // For now, just visual feedback is handled in Render()
    }

private:
    struct Padding
    {
        float horizontal = 10.0f;
        float vertical = 5.0f;
    } padding_;

    std::string text_;
    ColorScheme colors_;

    void UpdateSize()
    {
        // This is simplified - in a real implementation you'd measure the text
        // For now, use a rough estimate
        constexpr float char_width = 12.0f;   // Approximate character width
        constexpr float line_height = 24.0f;  // Approximate line height

        SetSize(text_.length() * char_width + 2 * padding_.horizontal,
                line_height + 2 * padding_.vertical);
    }

    [[nodiscard]] sdl::Color GetCurrentTextColor() const noexcept
    {
        const auto& state = GetState();

        if (!state.enabled) return colors_.disabled;
        if (state.pressed) return colors_.pressed;
        if (state.focused) return colors_.focused;
        if (state.hovered) return colors_.hovered;
        return colors_.normal;
    }
};

}  // namespace eerium::ui