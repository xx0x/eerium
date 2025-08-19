#include "ResourceManager.hpp"

#include <print>

namespace eerium::sdl
{

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

ResourceManager::~ResourceManager()
{
    Shutdown();
}

void ResourceManager::Initialize()
{
    if (initialized_)
    {
        return;  // Already initialized
    }

    if (!TTF_Init())
    {
        throw ResourceLoadException(
            std::string("SDL_ttf could not initialize! SDL_Error: ") + SDL_GetError());
    }

    initialized_ = true;
    std::println("ResourceManager: SDL_ttf initialized successfully");
}

void ResourceManager::Shutdown()
{
    if (!initialized_)
    {
        return;
    }

    // Clear all fonts first (they will auto-cleanup via RAII)
    fonts_.clear();

    // Then shutdown TTF
    TTF_Quit();
    initialized_ = false;
    std::println("ResourceManager: SDL_ttf shut down");
}

void ResourceManager::LoadFont(const std::string& name, const std::string& file_path, int point_size)
{
    if (!initialized_)
    {
        throw ResourceLoadException("ResourceManager not initialized");
    }

    auto font = std::make_unique<Font>();
    if (!font->LoadFromFile(file_path, point_size))
    {
        throw ResourceLoadException(
            std::string("Failed to load font '") + name + "' from '" + file_path + "'");
    }

    fonts_[name] = std::move(font);
    std::println("ResourceManager: Loaded font '{}' from '{}' at size {}",
                 name, file_path, point_size);
}

std::optional<Font> ResourceManager::GetFont(const std::string& name) const
{
    auto it = fonts_.find(name);
    if (it == fonts_.end())
    {
        return std::nullopt;
    }
    auto& font = it->second;
    if (!font->IsValid())
    {
        return std::nullopt;
    }
    // Return a copy of the font
    return *font;
}

bool ResourceManager::HasFont(const std::string& name) const
{
    return fonts_.find(name) != fonts_.end();
}

std::optional<Font> ResourceManager::GetDefaultFont() const
{
    return GetFont(kDefaultFontName);
}

}  // namespace eerium::sdl
