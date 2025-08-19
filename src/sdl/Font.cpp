#include "Font.hpp"

#include <SDL3/SDL.h>

#include <print>

namespace eerium::sdl
{

Font::Font(const std::string& file_path, int point_size)
    : file_path_(file_path), point_size_(point_size)
{
    LoadFromFile(file_path, point_size);
}

Font::Font(const Font& other)
    : file_path_(other.file_path_), point_size_(other.point_size_)
{
    if (other.IsValid())
    {
        LoadFromFile(file_path_, point_size_);
    }
}

Font& Font::operator=(const Font& other)
{
    if (this != &other)
    {
        Reset();
        file_path_ = other.file_path_;
        point_size_ = other.point_size_;
        if (other.IsValid())
        {
            LoadFromFile(file_path_, point_size_);
        }
    }
    return *this;
}

Font::Font(Font&& other) noexcept : font_(other.font_)
{
    other.font_ = nullptr;
}

Font& Font::operator=(Font&& other) noexcept
{
    if (this != &other)
    {
        Reset();
        font_ = other.font_;
        other.font_ = nullptr;
    }
    return *this;
}

Font::~Font()
{
    Reset();
}

bool Font::LoadFromFile(const std::string& file_path, int point_size)
{
    Reset();  // Clean up any existing font

    font_ = TTF_OpenFont(file_path.c_str(), point_size);
    if (!font_)
    {
        std::print(stderr, "Failed to load font '{}' at size {}: {}\n",
                   file_path, point_size, SDL_GetError());
        return false;
    }

    // Store the path and size for potential copying
    file_path_ = file_path;
    point_size_ = point_size;
    return true;
}

bool Font::IsValid() const noexcept
{
    return font_ != nullptr;
}

TTF_Font* Font::Get() const noexcept
{
    return font_;
}

Font::operator TTF_Font*() const noexcept
{
    return font_;
}

void Font::Reset()
{
    if (font_)
    {
        TTF_CloseFont(font_);
        font_ = nullptr;
    }
}

int Font::GetHeight() const noexcept
{
    return font_ ? TTF_GetFontHeight(font_) : 0;
}

int Font::GetAscent() const noexcept
{
    return font_ ? TTF_GetFontAscent(font_) : 0;
}

int Font::GetDescent() const noexcept
{
    return font_ ? TTF_GetFontDescent(font_) : 0;
}

}  // namespace eerium::sdl
