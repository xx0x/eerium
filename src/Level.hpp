#pragma once

#include <SDL3/SDL.h>

#include <array>
#include <print>
#include <random>

#include "objects/Player.hpp"
#include "objects/Rock.hpp"
#include "objects/Tree.hpp"

namespace eerium
{
class Level
{
public:
    Level()
    {
        Reset();
    }

    void Reset()
    {
        // Clear existing objects (if any)
        objects_.clear();

        // Set up randomizer
        std::random_device rd;
        // RANLUX random number generator (slow but high quality)
        std::ranlux24 gen(rd());
        // Uniform distributions for x and y coordinates
        std::uniform_int_distribution<int> distX(20, width_ - 20);
        std::uniform_int_distribution<int> distY(20, height_ - 20);

        // Create player
        player1_ = std::make_shared<objects::Player>("Hannah", sdl::kColorMagenta);
        player1_->SetPosition(400, 300);
        player1_->WalkTo({400, 300});
        std::println("Player initialized at position {}", player1_->GetPosition());
        objects_.push_back(player1_);

        // Create trees
        for (int i = 0; i < kTreesCount; ++i)
        {
            auto tree = std::make_shared<objects::Tree>();
            tree->SetPosition(distX(gen), distY(gen));
            objects_.push_back(tree);
        }
        std::println("Trees created");

        // Create rocks
        for (int i = 0; i < kRocksCount; ++i)
        {
            auto rock = std::make_shared<objects::Rock>();
            rock->SetPosition(distX(gen), distY(gen));
            objects_.push_back(rock);
        }
        std::println("Rocks created");
    }

    void Render(sdl::Renderer& renderer)
    {
        // Clear screen
        renderer.Clear(sdl::kColorBlack);

        // Draw the objects
        for (auto& object : objects_)
        {
            object->Render(renderer);
        }
    }

    void Update()
    {
        for (auto& object : objects_)
        {
            object->Update();
        }
    }

    void HandleEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
                case SDLK_UP:
                    PlayerMove(0, -10);
                    break;
                case SDLK_DOWN:
                    PlayerMove(0, 10);
                    break;
                case SDLK_LEFT:
                    PlayerMove(-10, 0);
                    break;
                case SDLK_RIGHT:
                    PlayerMove(10, 0);
                    break;
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                float mouse_x = event.button.x;
                float mouse_y = event.button.y;
                std::println("Mouse clicked at ({}, {})", mouse_x, mouse_y);
                if (player1_ != nullptr)
                {
                    player1_->WalkTo({mouse_x, mouse_y});
                }
            }
        }
    }

    void PlayerMove(float delta_x, float delta_y)
    {
        if (player1_ != nullptr)
        {
            player1_->WalkTo({player1_->GetPosition().x + delta_x,
                              player1_->GetPosition().y + delta_y});
        }
    }

private:
    // Main parameters
    float width_ = 800;
    float height_ = 600;
    static constexpr std::size_t kTreesCount = 10;
    static constexpr std::size_t kRocksCount = 10;

    // Game objects
    std::vector<std::shared_ptr<objects::BaseObject>> objects_;
    std::shared_ptr<objects::Player> player1_;
};

}  // namespace eerium