#pragma once

#include <array>
#include <cstdint>
#include <vector>

#include "sdl/Renderer.hpp"
#include "ui/BaseElement.hpp"

namespace eerium::ui
{

class Container
{
public:
    void AddItem(std::unique_ptr<BaseElement> element)
    {
        elements_.emplace_back(std::move(element));
    }

    void Render(sdl::Renderer& renderer)
    {
        float offset_y = position_y_;
        for (auto& element : elements_)
        {
            element->SetPosition(position_x_, offset_y);
            element->Render(renderer);
            offset_y += element->GetHeight();
        }
    }

    void HandleEvent(const SDL_Event& event)
    {
        for (auto& element : elements_)
        {
            element->HandleEvent(event);
        }
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            if (event.key.key == SDLK_UP)
            {
                selected_index_ = (selected_index_ - 1 + elements_.size()) % elements_.size();
            }
            else if (event.key.key == SDLK_DOWN)
            {
                selected_index_ = (selected_index_ + 1) % elements_.size();
            }
            else if (event.key.key == SDLK_RETURN)
            {
                if (selected_index_ >= 0)
                {
                    elements_[selected_index_]->TriggerOnClick();
                }
            }
        }
        for (int i = 0; i < elements_.size(); ++i)
        {
            elements_[i]->TriggerOnHover(i == selected_index_);
        }
    }

    const std::vector<std::unique_ptr<BaseElement>>& GetElements() const
    {
        return elements_;
    }

    void SetPosition(float x, float y)
    {
        position_x_ = x;
        position_y_ = y;
    }

private:
    int selected_index_ = -1;
    float position_x_ = 0.0f;
    float position_y_ = 0.0f;

    std::vector<std::unique_ptr<BaseElement>> elements_;
};

}  // namespace eerium::ui