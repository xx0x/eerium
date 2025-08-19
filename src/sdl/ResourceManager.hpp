#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <optional>

#include "Font.hpp"

namespace eerium::sdl
{

/**
 * @brief Exception thrown when resource loading fails
 */
class ResourceLoadException : public std::runtime_error
{
public:
    explicit ResourceLoadException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @brief Singleton resource manager for shared game resources
 *
 * Manages fonts, textures, and other shared resources that can be
 * accessed from anywhere in the game. Automatically handles SDL_ttf
 * initialization and cleanup.
 */
class ResourceManager
{
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the resource manager instance
     */
    static ResourceManager& Instance();

    /**
     * @brief Initialize SDL_ttf and prepare resource manager
     * @throws ResourceLoadException if initialization fails
     */
    void Initialize();

    /**
     * @brief Shutdown and cleanup all resources
     */
    void Shutdown();

    /**
     * @brief Load a font and store it with the given name
     * @param name Identifier for the font
     * @param file_path Path to the font file
     * @param point_size Size of the font in points
     * @throws ResourceLoadException if font loading fails
     */
    void LoadFont(const std::string& name, const std::string& file_path, int point_size);

    /**
     * @brief Get a loaded font by name
     * @param name Identifier of the font
     * @return Copy of the font, or error if not found/invalid
     */
    std::optional<Font> GetFont(const std::string& name) const;

    /**
     * @brief Check if a font is loaded
     * @param name Identifier of the font
     * @return true if font exists and is loaded
     */
    bool HasFont(const std::string& name) const;

    /**
     * @brief Get default UI font (convenience method)
     * @return Copy of the default UI font, or error if not found/invalid
     */
    std::optional<Font> GetDefaultFont() const;

    // Disable copy/move for singleton
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

private:
    ResourceManager() = default;
    ~ResourceManager();

    std::unordered_map<std::string, std::unique_ptr<Font>> fonts_;
    bool initialized_ = false;

    static constexpr const char* kDefaultFontName = "default";
};

}  // namespace eerium::sdl
