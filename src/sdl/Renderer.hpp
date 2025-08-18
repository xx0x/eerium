#pragma once
#include <SDL3/SDL.h>

namespace eerium::sdl
{

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

private:
    SDL_Renderer* renderer_ = nullptr;
};

}  // namespace eerium::sdl