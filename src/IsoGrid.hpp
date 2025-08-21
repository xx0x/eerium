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

    class Player
    {
    public:
        struct Position
        {
            float x;
            float y;
        };

        Player(const std::string& name, sdl::Color color)
        {
            name_ = name;
            color_ = color;
            position_ = {0.0f, 0.0f};
        }

        void Reset()
        {
            position_ = {0.0f, 0.0f};
        }

        void Move(float dx, float dy)
        {
            position_.x += dx;
            position_.y += dy;
        }

        Position GetPosition() const { return position_; }
        sdl::Color GetColor() const { return color_; }
        std::string GetName() const { return name_; }

    private:
        std::string name_;
        Position position_;
        sdl::Color color_;
    };

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

        // Reset player
        player_.Reset();
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
                    player_.Move(-1, -1);
                    break;
                case SDLK_DOWN:
                    player_.Move(1, 1);
                    break;
                case SDLK_LEFT:
                    player_.Move(-1, 1);
                    break;
                case SDLK_RIGHT:
                    player_.Move(1, -1);
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

        // Draw player using same isometric transformation as tiles
        float playerScreenX = (player_.GetPosition().x - player_.GetPosition().y) * (kTileWidth / 2.0f) + offset_x_;
        float playerScreenY = (player_.GetPosition().x + player_.GetPosition().y) * (kTileHeight / 2.0f) + offset_y_;

        // Player diamond vertices with same shape as tiles
        SDL_Vertex playerDiamond[4] = {
            {{playerScreenX, (playerScreenY - kTileHeight / 2.0f)},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}},  // top
            {{(playerScreenX + kTileWidth / 2.0f), playerScreenY},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}},  // right
            {{playerScreenX, (playerScreenY + kTileHeight / 2.0f)},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}},  // bottom
            {{(playerScreenX - kTileWidth / 2.0f), playerScreenY},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}}  // left
        };

        // Indices for 2 triangles making a diamond
        int playerIndices[] = {0, 1, 2, 0, 2, 3};

        SDL_RenderGeometry(renderer, nullptr,
                           playerDiamond, 4,
                           playerIndices, 6);
    };

private:
    std::array<std::array<Tile, kMapWidth>, kMapHeight> map_;
    float offset_x_ = 400.0f;
    float offset_y_ = 150.0f;
    Player player_ = {"Hannah", sdl::kColorMagenta};
};

}  // namespace eerium