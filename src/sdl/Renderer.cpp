#include "Renderer.hpp"

#include <print>

#include "Exception.hpp"

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

}  // namespace eerium::sdl
