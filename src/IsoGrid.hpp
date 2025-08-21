#pragma once

#include <cmath>
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

    // Camera deadzone - the screen is divided into this many parts, camera follows when player leaves center area
    static constexpr float kCameraDeadzoneDivisor = 5.0f;

    // Static helper functions for isometric coordinate transformations
    struct TileCoord
    {
        float x;
        float y;
    };

    struct PixelCoord
    {
        float x;
        float y;
    };

    // Convert tile coordinates to pixel coordinates
    PixelCoord TileToPixel(const TileCoord& tile) const
    {
        return {
            (tile.x - tile.y) * (kTileWidth / 2.0f) + offset_.x,
            (tile.x + tile.y) * (kTileHeight / 2.0f) + offset_.y};
    }

    // Convert pixel coordinates to tile coordinates
    TileCoord PixelToTile(const PixelCoord& pixel) const
    {
        float adjusted_x = pixel.x - offset_.x;
        float adjusted_y = pixel.y - offset_.y;

        return {
            (adjusted_x / (kTileWidth / 2.0f) + adjusted_y / (kTileHeight / 2.0f)) / 2.0f,
            (adjusted_y / (kTileHeight / 2.0f) - adjusted_x / (kTileWidth / 2.0f)) / 2.0f};
    }

    // Convenience overloads for direct float values
    PixelCoord TileToPixel(float tileX, float tileY) const
    {
        return TileToPixel({tileX, tileY});
    }

    TileCoord PixelToTile(float pixelX, float pixelY) const
    {
        return PixelToTile({pixelX, pixelY});
    }

    class Player
    {
    public:
        Player(const std::string& name, sdl::Color color)
        {
            name_ = name;
            color_ = color;
            position_ = {0.0f, 0.0f};
            target_position_ = {0.0f, 0.0f};
        }

        void Reset()
        {
            position_ = {0.0f, 0.0f};
            target_position_ = {0.0f, 0.0f};
        }

        void Update()
        {
            // Smooth movement towards target position
            static constexpr float kMovementSpeed = 5.0f;  // units per second
            static constexpr float kMinDistance = 0.01f;   // stop when very close
            // Move towards target (300 = fps)
            // TODO: Replace with actual delta time for frame rate independence
            static constexpr float kFrameTime = 1.0f / 300.0f;
            static constexpr float kMoveDistance = kMovementSpeed * kFrameTime;

            float dx = target_position_.x - position_.x;
            float dy = target_position_.y - position_.y;
            // Calculate distance to target
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance > kMinDistance)
            {
                // Normalize direction and apply speed
                float normalized_dx = dx / distance;
                float normalized_dy = dy / distance;

                // Don't overshoot the target
                if (kMoveDistance > distance)
                {
                    position_ = target_position_;
                }
                else
                {
                    position_.x += normalized_dx * kMoveDistance;
                    position_.y += normalized_dy * kMoveDistance;
                }
            }
        }

        /*
        // Alternative update method that accepts delta time for frame rate independence
        void Update(float deltaTime)
        {
            constexpr float kMovementSpeed = 5.0f; // units per second
            constexpr float kMinDistance = 0.01f;  // stop when very close

            float dx = target_position_.x - position_.x;
            float dy = target_position_.y - position_.y;

            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance > kMinDistance)
            {
                float normalized_dx = dx / distance;
                float normalized_dy = dy / distance;

                float moveDistance = kMovementSpeed * deltaTime;

                if (moveDistance > distance)
                {
                    position_ = target_position_;
                }
                else
                {
                    position_.x += normalized_dx * moveDistance;
                    position_.y += normalized_dy * moveDistance;
                }
            }
        }*/

        void MoveBy(float dx, float dy)
        {
            target_position_.x += dx;
            target_position_.y += dy;
        }

        void MoveTo(float x, float y, bool round = true)
        {
            if (round)
            {
                target_position_.x = std::round(x);
                target_position_.y = std::round(y);
            }
            else
            {
                target_position_.x = x;
                target_position_.y = y;
            }
        }

        TileCoord GetPosition() const { return position_; }
        sdl::Color GetColor() const { return color_; }
        std::string GetName() const { return name_; }

    private:
        std::string name_;
        TileCoord position_;
        TileCoord target_position_;
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
        player_.Update();

        // Camera following logic - keep player in center area
        PixelCoord player_screen_pos = TileToPixel(player_.GetPosition());

        // Calculate center area boundaries using the deadzone divisor
        // For divisor=5: center area is middle 1/5 of screen, camera follows in outer 4/5
        float margin_size = window_size_.x / kCameraDeadzoneDivisor;
        float center_left_bound = (window_size_.x - margin_size) / 2.0f;
        float center_right_bound = (window_size_.x + margin_size) / 2.0f;

        float margin_size_y = window_size_.y / kCameraDeadzoneDivisor;
        float center_top_bound = (window_size_.y - margin_size_y) / 2.0f;
        float center_bottom_bound = (window_size_.y + margin_size_y) / 2.0f;

        // Check if player is outside center area and adjust camera
        bool needs_camera_update = false;
        PixelCoord new_offset = offset_;

        if (player_screen_pos.x < center_left_bound)
        {
            new_offset.x = offset_.x + (center_left_bound - player_screen_pos.x);
            needs_camera_update = true;
        }
        else if (player_screen_pos.x > center_right_bound)
        {
            new_offset.x = offset_.x + (center_right_bound - player_screen_pos.x);
            needs_camera_update = true;
        }

        if (player_screen_pos.y < center_top_bound)
        {
            new_offset.y = offset_.y + (center_top_bound - player_screen_pos.y);
            needs_camera_update = true;
        }
        else if (player_screen_pos.y > center_bottom_bound)
        {
            new_offset.y = offset_.y + (center_bottom_bound - player_screen_pos.y);
            needs_camera_update = true;
        }

        if (needs_camera_update)
        {
            offset_ = new_offset;
        }
    }

    void HandleEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
                case SDLK_UP:
                    player_.MoveBy(-1, -1);
                    break;
                case SDLK_DOWN:
                    player_.MoveBy(1, 1);
                    break;
                case SDLK_LEFT:
                    player_.MoveBy(-1, 1);
                    break;
                case SDLK_RIGHT:
                    player_.MoveBy(1, -1);
                    break;
            }
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                // Convert mouse position to tile coordinates using helper function
                TileCoord tilePos = PixelToTile(event.button.x, event.button.y);
                player_.MoveTo(tilePos.x, tilePos.y);
            }
        }
    }

    void OffsetMove(float dx, float dy)
    {
        offset_.x += dx;
        offset_.y += dy;
    }

    void Render(sdl::Renderer& renderer)
    {
        // Update window size for camera system
        auto window_size_info = renderer.GetWindowSize();
        window_size_.x = static_cast<float>(window_size_info.width);
        window_size_.y = static_cast<float>(window_size_info.height);

        renderer.Clear(sdl::kColorDarkGrey);

        // Draw map
        for (int row = 0; row < kMapHeight; ++row)
        {
            for (int col = 0; col < kMapWidth; ++col)
            {
                PixelCoord pixel_pos = TileToPixel(col, row);
                float screen_x = pixel_pos.x;
                float screen_y = pixel_pos.y;

                // Diamond vertices with color data
                SDL_Vertex diamond[4] = {
                    {{screen_x, (screen_y - kTileHeight / 2.0f)},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}},  // top
                    {{(screen_x + kTileWidth / 2.0f), screen_y},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}},  // right
                    {{screen_x, (screen_y + kTileHeight / 2.0f)},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}},  // bottom
                    {{(screen_x - kTileWidth / 2.0f), screen_y},
                     {map_[row][col].color.r / 255.0f, map_[row][col].color.g / 255.0f,
                      map_[row][col].color.b / 255.0f, map_[row][col].color.a / 255.0f},
                     {0.0f, 0.0f}}  // left
                };

                // Indices for 2 triangles making a diamond
                int indices[] = {0, 1, 2, 0, 2, 3};  // unchanged

                SDL_RenderGeometry(renderer, nullptr,
                                   diamond, 4,
                                   indices, 6);
            }
        }

        // Draw player using same isometric transformation as tiles
        PixelCoord player_pixel_pos = TileToPixel(player_.GetPosition().x, player_.GetPosition().y);
        float player_screen_x = player_pixel_pos.x;
        float player_screen_y = player_pixel_pos.y;

        // Player diamond vertices with same shape as tiles
        SDL_Vertex player_diamond[4] = {
            {{player_screen_x, (player_screen_y - kTileHeight / 2.0f)},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}},  // top
            {{(player_screen_x + kTileWidth / 2.0f), player_screen_y},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}},  // right
            {{player_screen_x, (player_screen_y + kTileHeight / 2.0f)},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}},  // bottom
            {{(player_screen_x - kTileWidth / 2.0f), player_screen_y},
             {player_.GetColor().r / 255.0f, player_.GetColor().g / 255.0f,
              player_.GetColor().b / 255.0f, player_.GetColor().a / 255.0f},
             {0.0f, 0.0f}}  // left
        };

        // Indices for 2 triangles making a diamond
        int player_indices[] = {0, 1, 2, 0, 2, 3};

        SDL_RenderGeometry(renderer, nullptr,
                           player_diamond, 4,
                           player_indices, 6);
    };

private:
    std::array<std::array<Tile, kMapWidth>, kMapHeight> map_;
    PixelCoord offset_ = {400.0f, 150.0f};
    PixelCoord window_size_ = {800.0f, 600.0f};  // Default size, updated in Render()
    Player player_ = {"Hannah", sdl::kColorMagenta};
};

}  // namespace eerium