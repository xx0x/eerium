#pragma once

#include <iostream>
#include <vector>

#include "sdl/Color.hpp"
#include "sdl/Renderer.hpp"

namespace eerium
{

class IsoGrid
{
public:
    static constexpr float kTileWidth = 64.0f;   // diamond width
    static constexpr float kTileHeight = 32.0f;  // diamond height
    static constexpr int kMapWidth = 10;
    static constexpr int kMapHeight = 10;

    struct Tile
    {
        sdl::Color color;
    };

    IsoGrid()
    {
        // Make a simple map with alternating colors
        for (int r = 0; r < kMapHeight; ++r)
        {
            for (int c = 0; c < kMapWidth; ++c)
            {
                if ((r + c) % 2 == 0)
                    map_[r][c].color = {50, 200, 50, 255};  // green
                else
                    map_[r][c].color = {200, 200, 50, 255};  // yellow
            }
        }
    }

    void Update()
    {
    }

    void HandleEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
                case SDLK_UP:
                    OffsetMove(0, -10);
                    break;
                case SDLK_DOWN:
                    OffsetMove(0, 10);
                    break;
                case SDLK_LEFT:
                    OffsetMove(-10, 0);
                    break;
                case SDLK_RIGHT:
                    OffsetMove(10, 0);
                    break;
            }
        }
    }

    void OffsetMove(float dx, float dy)
    {
        offset_x_ += dx;
        offset_y_ += dy;
    }

    void Render(sdl::Renderer& renderer)
    {
        renderer.Clear(sdl::kColorDarkGrey);

        // Draw map
        for (int row = 0; row < kMapHeight; ++row)
        {
            for (int col = 0; col < kMapWidth; ++col)
            {
                float screenX = (col - row) * (kTileWidth / 2.0f) + offset_x_;
                float screenY = (col + row) * (kTileHeight / 2.0f) + offset_y_;

                // Diamond vertices with color data
                SDL_Vertex diamond[4] = {
                    {{screenX, (screenY - kTileHeight / 2.0f)},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}},  // top
                    {{(screenX + kTileWidth / 2.0f), screenY},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}},  // right
                    {{screenX, (screenY + kTileHeight / 2.0f)},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}},  // bottom
                    {{(screenX - kTileWidth / 2.0f), screenY},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}}  // left
                };

                // Indices for 2 triangles making a diamond
                int indices[] = {0, 1, 2, 0, 2, 3};

                SDL_RenderGeometry(renderer, nullptr,
                                   diamond, 4,
                                   indices, 6);
            }
        }
    };

private:
    std::array<std::array<Tile, kMapWidth>, kMapHeight> map_;
    float offset_x_ = 400.0f;
    float offset_y_ = 150.0f;
};

}  // namespace eerium