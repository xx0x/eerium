#pragma once

#include <SDL3/SDL.h>

namespace eerium::sdl
{

using Color = SDL_Color;

static constexpr Color kColorRed = {255, 0, 0, 255};
static constexpr Color kColorGreen = {0, 255, 0, 255};
static constexpr Color kColorBlue = {0, 0, 255, 255};
static constexpr Color kColorWhite = {255, 255, 255, 255};
static constexpr Color kColorBlack = {0, 0, 0, 255};
static constexpr Color kColorMagenta = {255, 0, 255, 255};
static constexpr Color kColorTransparent = {0, 0, 0, 0};
static constexpr Color kColorLightGrey = {200, 200, 200, 255};
static constexpr Color kColorYellow = {255, 255, 0, 255};
static constexpr Color kColorDarkGrey = {80, 80, 80, 255};

}  // namespace eerium::sdl
