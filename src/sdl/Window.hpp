#pragma once

#include <SDL3/SDL.h>

#include <string>

namespace eerium::sdl
{

/**
 * @brief RAII wrapper for SDL_Window
 */
class Window
{
public:
    Window(const std::string& title, int width, int height, Uint32 flags = 0);
    ~Window();

    // Move semantics
    Window(Window&& other) noexcept;
    Window& operator=(Window&& other) noexcept;

    // Disable copy
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    SDL_Window* Get() const noexcept { return window_; }
    operator SDL_Window*() const noexcept { return window_; }

private:
    SDL_Window* window_ = nullptr;
};

}  // namespace eerium::sdl