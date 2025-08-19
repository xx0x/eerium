#include "Renderer.hpp"

#include <SDL3_ttf/SDL_ttf.h>

#include <print>

#include "Exception.hpp"
#include "Font.hpp"

namespace eerium::sdl
{

// Renderer implementation
Renderer::Renderer(SDL_Window* window, const char* driver)
{
    renderer_ = SDL_CreateRenderer(window, driver);
    if (!renderer_)
    {
        throw Exception(
            std::string("Renderer could not be created! SDL_Error: ") + SDL_GetError());
    }
}

Renderer::~Renderer()
{
    if (renderer_)
    {
        SDL_DestroyRenderer(renderer_);
    }
}

Renderer::Renderer(Renderer&& other) noexcept : renderer_(other.renderer_)
{
    other.renderer_ = nullptr;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept
{
    if (this != &other)
    {
        if (renderer_)
        {
            SDL_DestroyRenderer(renderer_);
        }
        renderer_ = other.renderer_;
        other.renderer_ = nullptr;
    }
    return *this;
}

void Renderer::Clear(sdl::Color color)
{
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer_);
}

Renderer::WindowSize Renderer::GetWindowSize() const
{
    // Get window size for centering
    SDL_Window* window = SDL_GetRenderWindow(renderer_);
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);
    return {static_cast<float>(window_width), static_cast<float>(window_height)};
}

void Renderer::RenderText(const std::string& text,
                          float x,
                          float y,
                          Color color,
                          const Font& font,
                          TextAlign align)
{
    if (!font.IsValid())
    {
        return;
    }

    SDL_Surface* text_surface = TTF_RenderText_Solid(font.Get(), text.c_str(), text.length(), color);
    if (!text_surface)
    {
        std::print(stderr, "Unable to render text surface! SDL_Error: {}\n", SDL_GetError());
        return;
    }

    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer_, text_surface);
    if (!text_texture)
    {
        std::print(stderr, "Unable to create texture from rendered text! SDL_Error: {}\n", SDL_GetError());
        SDL_DestroySurface(text_surface);
        return;
    }

    float text_width = text_surface->w;
    float text_height = text_surface->h;
    SDL_DestroySurface(text_surface);

    // Calculate x position based on alignment
    float render_x = x;

    switch (align)
    {
        case TextAlign::LEFT:
            // x is already the left position
            break;
        case TextAlign::CENTER:
            render_x = x - text_width / 2;
            break;
        case TextAlign::RIGHT:
            render_x = x - text_width;
            break;
    }

    SDL_FRect render_quad = {render_x, y, text_width, text_height};
    SDL_RenderTexture(renderer_, text_texture, nullptr, &render_quad);
    SDL_DestroyTexture(text_texture);
}

}  // namespace eerium::sdl
