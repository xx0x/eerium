#pragma once
#include <SDL3/SDL.h>

namespace eerium::sdl
{

/**
 * @brief RAII wrapper for SDL initialization
 */
class Context
{
public:
    explicit Context(Uint32 flags = SDL_INIT_VIDEO);
    ~Context();

    // Disable copy and move for singleton-like behavior
    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;

private:
    bool initialized_ = false;
};

}  // namespace eerium::sdl
