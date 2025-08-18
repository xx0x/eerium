#include "Window.hpp"

#include <print>

#include "Exception.hpp"

namespace eerium::sdl
{

// Window implementation
Window::Window(const std::string& title, int width, int height, Uint32 flags)
{
    window_ = SDL_CreateWindow(title.c_str(), width, height, flags);
    if (!window_)
    {
        throw Exception(
            std::string("Window could not be created! SDL_Error: ") + SDL_GetError());
    }
}

Window::~Window()
{
    if (window_)
    {
        SDL_DestroyWindow(window_);
    }
}

Window::Window(Window&& other) noexcept : window_(other.window_)
{
    other.window_ = nullptr;
}

Window& Window::operator=(Window&& other) noexcept
{
    if (this != &other)
    {
        if (window_)
        {
            SDL_DestroyWindow(window_);
        }
        window_ = other.window_;
        other.window_ = nullptr;
    }
    return *this;
}

}  // namespace eerium::sdl