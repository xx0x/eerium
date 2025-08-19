#pragma once
#include <SDL3/SDL.h>

#include <string>

#include "Color.hpp"

namespace eerium::sdl
{

class Font;

/**
 * @brief RAII wrapper for SDL_Renderer
 */
class Renderer
{
public:
    explicit Renderer(SDL_Window* window, const char* driver = nullptr);
    ~Renderer();

    // Move semantics
    Renderer(Renderer&& other) noexcept;
    Renderer& operator=(Renderer&& other) noexcept;

    // Disable copy
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    SDL_Renderer* Get() const noexcept { return renderer_; }
    operator SDL_Renderer*() const noexcept { return renderer_; }

    enum class TextAlign
    {
        LEFT,
        CENTER,
        RIGHT
    };

    struct WindowSize
    {
        float width;
        float height;
    };

    /**
     * @brief Render text using the provided font
     * @param text The text to render
     * @param x X position for the text
     * @param y Y position for the text
     * @param color Color of the text
     * @param font Font to use for rendering
     * @param align Text alignment (default: LEFT)
     */
    void RenderText(const std::string& text, float x, float y, Color color,
                    const Font& font, TextAlign align = TextAlign::LEFT);

    void Clear(Color color = {0, 0, 0, 255});

    WindowSize GetWindowSize() const;

private:
    SDL_Renderer* renderer_ = nullptr;
};

}  // namespace eerium::sdl