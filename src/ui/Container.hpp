#pragma once

#include <memory>
#include <vector>
#include <optional>
#include <algorithm>

#include "sdl/Renderer.hpp"
#include "ui/BaseElement.hpp"

namespace eerium::ui
{

enum class NavigationDirection {
    Up,
    Down,
    Left,
    Right
};

class Container
{
public:
    // Modern construction with move semantics and perfect forwarding
    template<typename ElementType, typename... Args>
    ElementType& EmplaceElement(Args&&... args) {
        auto element = std::make_unique<ElementType>(std::forward<Args>(args)...);
        ElementType& ref = *element;
        elements_.emplace_back(std::move(element));
        return ref;
    }
    
    void AddElement(std::unique_ptr<BaseElement> element) {
        if (element) {
            elements_.emplace_back(std::move(element));
        }
    }
    
    void Clear() {
        elements_.clear();
        selected_index_ = std::nullopt;
        hovered_index_ = std::nullopt;
    }

    void Render(sdl::Renderer& renderer) {
        UpdateLayout();
        for (const auto& element : elements_) {
            element->Render(renderer);
        }
    }

    void HandleEvent(const SDL_Event& event) {
        switch (event.type) {
            case SDL_EVENT_MOUSE_MOTION:
                HandleMouseMotion(event.motion.x, event.motion.y);
                break;
                
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                HandleMouseClick(event.button.x, event.button.y);
                break;
                
            case SDL_EVENT_KEY_DOWN:
                HandleKeyboard(event.key.key);
                break;
        }
    }

    // Modern accessors
    [[nodiscard]] const std::vector<std::unique_ptr<BaseElement>>& GetElements() const noexcept {
        return elements_;
    }
    
    [[nodiscard]] std::optional<size_t> GetSelectedIndex() const noexcept {
        return selected_index_;
    }
    
    [[nodiscard]] std::optional<size_t> GetHoveredIndex() const noexcept {
        return hovered_index_;
    }
    
    void SetPosition(float x, float y) noexcept {
        position_.x = x;
        position_.y = y;
    }
    
    void SetSpacing(float spacing) noexcept {
        spacing_ = spacing;
    }
    
    // Navigation control
    void SelectNext() { Navigate(NavigationDirection::Down); }
    void SelectPrevious() { Navigate(NavigationDirection::Up); }
    void SelectIndex(size_t index) {
        if (index < elements_.size()) {
            SetSelection(index);
        }
    }
    
    void ActivateSelected() {
        if (selected_index_ && *selected_index_ < elements_.size()) {
            elements_[*selected_index_]->TriggerClick();
        }
    }

private:
    struct Position {
        float x = 0.0f;
        float y = 0.0f;
    } position_;
    
    float spacing_ = 5.0f;
    std::vector<std::unique_ptr<BaseElement>> elements_;
    std::optional<size_t> selected_index_;
    std::optional<size_t> hovered_index_;
    
    void UpdateLayout() {
        float current_y = position_.y;
        for (const auto& element : elements_) {
            element->SetPosition(position_.x, current_y);
            current_y += element->GetHeight() + spacing_;
        }
    }
    
    void HandleMouseMotion(float x, float y) {
        std::optional<size_t> new_hovered;
        
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (elements_[i]->IsPointInside(x, y)) {
                new_hovered = i;
                break;
            }
        }
        
        if (new_hovered != hovered_index_) {
            SetHover(new_hovered);
            // Auto-select on hover for keyboard navigation consistency
            SetSelection(new_hovered);
        }
    }
    
    void HandleMouseClick(float x, float y) {
        for (size_t i = 0; i < elements_.size(); ++i) {
            if (elements_[i]->IsPointInside(x, y)) {
                SetSelection(i);
                elements_[i]->TriggerClick();
                break;
            }
        }
    }
    
    void HandleKeyboard(int key) {
        switch (key) {
            case SDLK_UP:
                Navigate(NavigationDirection::Up);
                break;
            case SDLK_DOWN:
                Navigate(NavigationDirection::Down);
                break;
            case SDLK_RETURN:
            case SDLK_SPACE:
                ActivateSelected();
                break;
        }
    }
    
    void Navigate(NavigationDirection direction) {
        if (elements_.empty()) return;
        
        size_t new_index;
        if (!selected_index_) {
            // No selection, select first or last based on direction
            new_index = (direction == NavigationDirection::Up) ? elements_.size() - 1 : 0;
        } else {
            switch (direction) {
                case NavigationDirection::Up:
                    new_index = (*selected_index_ == 0) ? elements_.size() - 1 : *selected_index_ - 1;
                    break;
                case NavigationDirection::Down:
                    new_index = (*selected_index_ + 1) % elements_.size();
                    break;
                default:
                    return; // Left/Right not implemented for vertical layout
            }
        }
        
        SetSelection(new_index);
        // Clear mouse hover when using keyboard
        SetHover(std::nullopt);
    }
    
    void SetSelection(std::optional<size_t> index) {
        if (selected_index_ == index) return;
        
        selected_index_ = index;
        UpdateElementStates();
    }
    
    void SetHover(std::optional<size_t> index) {
        if (hovered_index_ == index) return;
        
        hovered_index_ = index;
        UpdateElementStates();
    }
    
    void UpdateElementStates() {
        for (size_t i = 0; i < elements_.size(); ++i) {
            ElementState state;
            state.hovered = (hovered_index_ && *hovered_index_ == i);
            state.focused = (selected_index_ && *selected_index_ == i);
            state.enabled = true; // Could be configurable per element
            
            elements_[i]->SetState(state);
        }
    }
};

}  // namespace eerium::ui