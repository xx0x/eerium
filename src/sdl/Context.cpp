#include "Context.hpp"

#include <print>

#include "Exception.hpp"

namespace eerium::sdl
{

// Context implementation
Context::Context(Uint32 flags)
{
    if (!SDL_Init(flags))
    {
        throw Exception(
            std::string("SDL could not initialize! SDL_Error: ") + SDL_GetError());
    }
    initialized_ = true;
    std::print("SDL initialized successfully\n");
}

Context::~Context()
{
    if (initialized_)
    {
        SDL_Quit();
        std::print("SDL shut down\n");
    }
}

}  // namespace eerium::sdl