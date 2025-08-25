#pragma once

#include <string>
#include <string_view>

#include "sdl/Color.hpp"
#include "sdl/ResourceManager.hpp"
#include "ui/BaseElement.hpp"
#include <SDL3_ttf/SDL_ttf.h>

namespace eerium::ui
{

enum class TextAlignment {
    Left,
    Center,
    Right
};

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
    
    void SetTextAlignment(TextAlignment alignment) noexcept
    {
        alignment_ = alignment;
    }
    
    [[nodiscard]] TextAlignment GetTextAlignment() const noexcept { return alignment_; }

    void Render(sdl::Renderer& renderer) override
    {
        auto font = sdl::ResourceManager::Instance().GetDefaultFont();
        if (!font || !font->IsValid()) return;

        // Choose text color based on state
        const sdl::Color text_color = GetCurrentTextColor();

        // Create text surface using SDL_ttf directly
        SDL_Surface* text_surface = TTF_RenderText_Blended(font->Get(), text_.c_str(), text_.length(), text_color);
        if (!text_surface) return;

        // Get actual text dimensions from the surface
        float actual_text_width = static_cast<float>(text_surface->w);
        float actual_text_height = static_cast<float>(text_surface->h);

        // Create texture from surface
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer.Get(), text_surface);
        SDL_DestroySurface(text_surface);
        
        if (!text_texture) return;

        // Calculate text position based on alignment using actual dimensions
        float text_x, text_y;
        CalculateTextPositionWithDimensions(text_x, text_y, actual_text_width, actual_text_height);

        // Render the texture with actual dimensions
        SDL_FRect dest_rect = {text_x, text_y, actual_text_width, actual_text_height};
        SDL_RenderTexture(renderer.Get(), text_texture, nullptr, &dest_rect);

        SDL_DestroyTexture(text_texture);
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
    TextAlignment alignment_ = TextAlignment::Center;
    int text_width_ = 0;
    int text_height_ = 0;

    void UpdateSize()
    {
        auto font = sdl::ResourceManager::Instance().GetDefaultFont();
        if (!font || !font->IsValid()) {
            // Fallback to approximate size
            constexpr float char_width = 12.0f;
            constexpr float line_height = 24.0f;
            text_width_ = static_cast<int>(text_.length() * char_width);
            text_height_ = static_cast<int>(line_height);
        } else {
            // Use the same method as rendering to get consistent dimensions
            sdl::Color dummy_color = {255, 255, 255, 255}; // White color for measurement
            SDL_Surface* text_surface = TTF_RenderText_Blended(font->Get(), text_.c_str(), text_.length(), dummy_color);
            if (text_surface) {
                text_width_ = text_surface->w;
                text_height_ = text_surface->h;
                SDL_DestroySurface(text_surface);
            } else {
                // Fallback if surface creation fails
                text_width_ = static_cast<int>(text_.length() * 12.0f);
                text_height_ = font->GetHeight();
            }
        }

        SetSize(text_width_ + 2 * padding_.horizontal,
                text_height_ + 2 * padding_.vertical);
    }
    
    void CalculateTextPosition(float& text_x, float& text_y) const
    {
        text_y = GetY() + padding_.vertical;
        
        switch (alignment_) {
            case TextAlignment::Left:
                text_x = GetX() + padding_.horizontal;
                break;
            case TextAlignment::Center:
                text_x = GetX() + (GetWidth() - text_width_) / 2.0f;
                break;
            case TextAlignment::Right:
                text_x = GetX() + GetWidth() - text_width_ - padding_.horizontal;
                break;
        }
    }
    
    void CalculateTextPositionWithDimensions(float& text_x, float& text_y, float actual_width, float actual_height) const
    {
        text_y = GetY() + padding_.vertical;
        
        switch (alignment_) {
            case TextAlignment::Left:
                text_x = GetX() + padding_.horizontal;
                break;
            case TextAlignment::Center:
                text_x = GetX() + (GetWidth() - actual_width) / 2.0f;
                break;
            case TextAlignment::Right:
                text_x = GetX() + GetWidth() - actual_width - padding_.horizontal;
                break;
        }
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