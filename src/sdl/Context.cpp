#include "Context.hpp"

#include <print>

#include "Exception.hpp"
#include "ResourceManager.hpp"

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
    // Initialize resource manager and load default font
    ResourceManager::Instance().Initialize();
    ResourceManager::Instance().LoadFont("default",
                                         "../resources/fonts/UncialAntiqua-Regular.ttf",
                                         24);

    ResourceManager::Instance().LoadFont("title",
                                         "../resources/fonts/UncialAntiqua-Regular.ttf",
                                         48);

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