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
        // Initialize game objects
        player1_.SetPosition(400, 300);
        player1_.WalkTo({400, 300});
        std::println("Player initialized at position {}", player1_.GetPosition());

        // Randomize tree positions
        std::random_device rd;
        std::ranlux24 gen(rd());  // RANLUX random number generator (slow but high quality)

        // Uniform distributions for x and y coordinates
        std::uniform_int_distribution<int> distX(20, width_ - 20);
        std::uniform_int_distribution<int> distY(20, height_ - 20);

        for (auto& tree : trees_)
        {
            tree.SetPosition(distX(gen), distY(gen));
        }
        std::println("Trees randomized");

        for (auto& rock : rocks_)
        {
            rock.SetPosition(distX(gen), distY(gen));
        }
        std::println("Rocks randomized");
    }

    void Render(sdl::Renderer& renderer)
    {
        // Clear screen
        renderer.Clear(sdl::kColorBlack);

        // Draw the rocks
        for (auto& rock : rocks_)
        {
            rock.Render(renderer);
        }

        // Draw the player
        player1_.Render(renderer);

        // Draw the trees
        for (auto& tree : trees_)
        {
            tree.Render(renderer);
        }
    }

    void Update()
    {
        player1_.Update();
        for (auto& tree : trees_)
        {
            tree.Update();
        }
        for (auto& rock : rocks_)
        {
            rock.Update();
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
                player1_.WalkTo({mouse_x, mouse_y});
            }
        }
    }

    void PlayerMove(float delta_x, float delta_y)
    {
        player1_.WalkTo({player1_.GetPosition().x + delta_x,
                         player1_.GetPosition().y + delta_y});
    }

private:

    // Main parameters
    float width_ = 800;
    float height_ = 600;

    // Game objects
    objects::Player player1_ = {"Hannah", sdl::kColorMagenta};
    std::array<objects::Tree, 10> trees_;
    std::array<objects::Rock, 10> rocks_;
};

}  // namespace eerium