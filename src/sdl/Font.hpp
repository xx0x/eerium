#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <string>

namespace eerium::sdl
{

/**
 * @brief RAII wrapper for TTF_Font resources
 *
 * Automatically manages the lifetime of SDL_ttf font resources,
 * ensuring proper cleanup and providing a safe, modern C++ interface.
 */
class Font
{
public:
    /**
     * @brief Default constructor - creates an empty font
     */
    Font() = default;

    /**
     * @brief Load font from file path
     * @param file_path Path to the font file
     * @param point_size Size of the font in points
     */
    Font(const std::string& file_path, int point_size);

    /**
     * @brief Copy constructor - creates a new font with the same properties
     */
    Font(const Font& other);

    /**
     * @brief Copy assignment operator
     */
    Font& operator=(const Font& other);

    /**
     * @brief Move constructor
     */
    Font(Font&& other) noexcept;

    /**
     * @brief Move assignment operator
     */
    Font& operator=(Font&& other) noexcept;

    /**
     * @brief Destructor - automatically releases font resources
     */
    ~Font();

    /**
     * @brief Get the path and size used to load this font (for copying)
     */
    const std::string& GetFilePath() const noexcept { return file_path_; }
    int GetPointSize() const noexcept { return point_size_; }

    /**
     * @brief Load font from file path
     * @param file_path Path to the font file
     * @param point_size Size of the font in points
     * @return true if loading succeeded, false otherwise
     */
    bool LoadFromFile(const std::string& file_path, int point_size);

    /**
     * @brief Check if font is valid/loaded
     * @return true if font is loaded and ready to use
     */
    bool IsValid() const noexcept;

    /**
     * @brief Get the raw TTF_Font pointer for SDL functions
     * @return Raw pointer to TTF_Font, or nullptr if not loaded
     */
    TTF_Font* Get() const noexcept;

    /**
     * @brief Get the raw TTF_Font pointer for SDL functions (alternative syntax)
     * @return Raw pointer to TTF_Font, or nullptr if not loaded
     */
    operator TTF_Font*() const noexcept;

    /**
     * @brief Release the font and free resources
     */
    void Reset();

    /**
     * @brief Get font height
     * @return Height of the font in pixels, or 0 if not loaded
     */
    int GetHeight() const noexcept;

    /**
     * @brief Get font ascent
     * @return Ascent of the font in pixels, or 0 if not loaded
     */
    int GetAscent() const noexcept;

    /**
     * @brief Get font descent
     * @return Descent of the font in pixels, or 0 if not loaded
     */
    int GetDescent() const noexcept;

private:
    TTF_Font* font_ = nullptr;
    std::string file_path_;
    int point_size_ = 0;
};

}  // namespace eerium::sdl
