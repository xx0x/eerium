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
    std::print("ResourceManager: SDL_ttf initialized successfully\n");
}

void ResourceManager::Shutdown()
{
    if (!initialized_)
    {
        return;
    }

    // Clear all fonts (they will auto-cleanup via RAII)
    fonts_.clear();

    TTF_Quit();
    initialized_ = false;
    std::print("ResourceManager: SDL_ttf shut down\n");
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
    std::print("ResourceManager: Loaded font '{}' from '{}' at size {}\n",
               name, file_path, point_size);
}

const Font& ResourceManager::GetFont(const std::string& name) const
{
    auto it = fonts_.find(name);
    if (it == fonts_.end())
    {
        throw std::out_of_range("Font '" + name + "' not found");
    }
    return *it->second;
}

bool ResourceManager::HasFont(const std::string& name) const
{
    return fonts_.find(name) != fonts_.end();
}

const Font* ResourceManager::GetDefaultFont() const
{
    auto it = fonts_.find(kDefaultFontName);
    if (it == fonts_.end())
    {
        return nullptr;
    }
    
    const Font* font = it->second.get();
    if (!font->IsValid())
    {
        return nullptr;
    }
    
    return font;
}

}  // namespace eerium::sdl
