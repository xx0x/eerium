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

    static constexpr sdl::Color kHoverColor = {255u, 255u, 255u, 100u};

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
    TileCoord PixelToTile(const PixelCoord& pixel, bool round = false) const
    {
        float adjusted_x = pixel.x - offset_.x;
        float adjusted_y = pixel.y - offset_.y;

        TileCoord result = {
            (adjusted_x / (kTileWidth / 2.0f) + adjusted_y / (kTileHeight / 2.0f)) / 2.0f,
            (adjusted_y / (kTileHeight / 2.0f) - adjusted_x / (kTileWidth / 2.0f)) / 2.0f};
        if (round)
        {
            result.x = std::round(result.x);
            result.y = std::round(result.y);
        }
        return result;
    }

    // Convenience overloads for direct float values
    PixelCoord TileToPixel(float tileX, float tileY) const
    {
        return TileToPixel({tileX, tileY});
    }

    TileCoord PixelToTile(float pixelX, float pixelY, bool round = false) const
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

        void MoveTo(float x, float y, bool round = false)
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

    enum class Material
    {
        GRASS,
        DIRT,
        STONE,
        WATER
    };

    struct Tile
    {
        Material material = Material::GRASS;
    };

    static constexpr sdl::Color MaterialToColor(Material material)
    {
        switch (material)
        {
            case Material::GRASS:
                return {50, 200, 50, 255};
            case Material::DIRT:
                return {150, 100, 50, 255};
            case Material::STONE:
                return {150, 150, 150, 255};
            case Material::WATER:
                return {50, 50, 200, 255};
            default:
                return {0, 0, 0, 255};  // black for unknown
        }
    }

    IsoGrid()
    {
        Reset();
    }

    void Reset()
    {
        // Make a simple map
        for (int r = 0; r < kMapHeight; ++r)
        {
            for (int c = 0; c < kMapWidth; ++c)
            {
                Tile& tile = map_[r][c];
                tile.material = Material::GRASS;
                if (rand() % 8 == 0)
                {
                    tile.material = Material::DIRT;
                }
                else if (rand() % 7 == 0)
                {
                    tile.material = Material::STONE;
                }
            }
        }

        // Reset player
        player_.Reset();
    }

    void Update()
    {
        player_.Update();
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
                player_.MoveTo(tilePos.x, tilePos.y, true);
            }
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            mouse_position_.x = event.motion.x;
            mouse_position_.y = event.motion.y;
            mouse_position_valid_ = true;
        }
    }

    void OffsetMove(float dx, float dy)
    {
        offset_.x += dx;
        offset_.y += dy;
    }

    void DrawTile(sdl::Renderer& renderer, const TileCoord& position, sdl::Color color)
    {
        PixelCoord pixel_pos = TileToPixel(position);
        float screen_x = pixel_pos.x;
        float screen_y = pixel_pos.y;

        SDL_FColor fcolor = {color.r / 255.0f, color.g / 255.0f,
                             color.b / 255.0f, color.a / 255.0f};

        // Diamond vertices with color data
        SDL_Vertex diamond[4] = {
            {{screen_x, (screen_y - kTileHeight / 2.0f)}, fcolor, {0.0f, 0.0f}},  // top
            {{(screen_x + kTileWidth / 2.0f), screen_y}, fcolor, {0.0f, 0.0f}},   // right
            {{screen_x, (screen_y + kTileHeight / 2.0f)}, fcolor, {0.0f, 0.0f}},  // bottom
            {{(screen_x - kTileWidth / 2.0f), screen_y}, fcolor, {0.0f, 0.0f}}    // left
        };

        // Indices for 2 triangles making a diamond
        int indices[] = {0, 1, 2, 0, 2, 3};
        SDL_RenderGeometry(renderer, nullptr, diamond, 4, indices, 6);
    }

    void UpdateCameraBounds(sdl::Renderer& renderer)
    {
        // Update window size for camera system
        auto window_size = renderer.GetWindowSize();

        // Camera following logic - keep player in center area
        PixelCoord player_screen_pos = TileToPixel(player_.GetPosition());

        // Calculate center area boundaries using the deadzone divisor
        // For divisor=5: center area is middle 1/5 of screen, camera follows in outer 4/5
        float margin_size = window_size.width / kCameraDeadzoneDivisor;
        float center_left_bound = (window_size.width - margin_size) / 2.0f;
        float center_right_bound = (window_size.width + margin_size) / 2.0f;

        float margin_size_y = window_size.height / kCameraDeadzoneDivisor;
        float center_top_bound = (window_size.height - margin_size_y) / 2.0f;
        float center_bottom_bound = (window_size.height + margin_size_y) / 2.0f;

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
            // mouse_position_valid_ = false;
        }
    }

    void Render(sdl::Renderer& renderer)
    {
        // Update camera bounds
        UpdateCameraBounds(renderer);

        // Clear renderer
        renderer.Clear(sdl::kColorDarkGrey);

        // Draw map
        for (int row = 0; row < kMapHeight; ++row)
        {
            for (int col = 0; col < kMapWidth; ++col)
            {
                Tile& tile = map_[row][col];
                TileCoord coord = {static_cast<float>(col), static_cast<float>(row)};
                DrawTile(renderer, coord, MaterialToColor(tile.material));
            }
        }

        // Draw player
        DrawTile(renderer, player_.GetPosition(), player_.GetColor());

        // Draw mouse hover
        if (mouse_position_valid_)
        {
            DrawTile(renderer, PixelToTile(mouse_position_, true), kHoverColor);
        }
    };

private:
    std::array<std::array<Tile, kMapWidth>, kMapHeight> map_;
    PixelCoord offset_ = {400.0f, 150.0f};
    Player player_ = {"Hannah", {255u, 0u, 255u, 200u}};
    PixelCoord mouse_position_ = {0.0f, 0.0f};
    bool mouse_position_valid_ = false;
};

}  // namespace eerium