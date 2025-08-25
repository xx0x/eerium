#pragma once

#include <functional>
#include <memory>

#include "sdl/Renderer.hpp"

namespace eerium::ui
{

// Forward declaration
class Container;

// Modern event system using std::function with better type safety
using ClickHandler = std::function<void()>;
using HoverHandler = std::function<void(bool hovered)>;
using FocusHandler = std::function<void(bool focused)>;

struct ElementState {
    bool hovered = false;
    bool focused = false;
    bool pressed = false;
    bool enabled = true;
};

class BaseElement
{
public:
    virtual ~BaseElement() = default;
    
    // Pure virtual methods that must be implemented
    virtual void Render(sdl::Renderer& renderer) = 0;
    virtual void OnStateChanged(const ElementState& old_state, const ElementState& new_state) {}
    
    // Modern positioning with better encapsulation
    void SetPosition(float x, float y) noexcept { 
        position_.x = x; 
        position_.y = y; 
    }
    
    void SetSize(float width, float height) noexcept { 
        size_.width = width; 
        size_.height = height; 
    }
    
    // Getters with proper const-correctness
    [[nodiscard]] constexpr float GetX() const noexcept { return position_.x; }
    [[nodiscard]] constexpr float GetY() const noexcept { return position_.y; }
    [[nodiscard]] constexpr float GetWidth() const noexcept { return size_.width; }
    [[nodiscard]] constexpr float GetHeight() const noexcept { return size_.height; }
    
    [[nodiscard]] bool IsPointInside(float x, float y) const noexcept {
        return (x >= position_.x && x <= position_.x + size_.width &&
                y >= position_.y && y <= position_.y + size_.height);
    }
    
    // Modern event handlers with move semantics
    void SetClickHandler(ClickHandler&& handler) { click_handler_ = std::move(handler); }
    void SetHoverHandler(HoverHandler&& handler) { hover_handler_ = std::move(handler); }
    void SetFocusHandler(FocusHandler&& handler) { focus_handler_ = std::move(handler); }
    
    // State management - only Container should call these
    void SetState(const ElementState& new_state) {
        if (state_.hovered != new_state.hovered || 
            state_.focused != new_state.focused ||
            state_.pressed != new_state.pressed ||
            state_.enabled != new_state.enabled) {
            
            ElementState old_state = state_;
            state_ = new_state;
            
            // Notify derived classes
            OnStateChanged(old_state, state_);
            
            // Call handlers
            if (old_state.hovered != state_.hovered && hover_handler_) {
                hover_handler_(state_.hovered);
            }
            if (old_state.focused != state_.focused && focus_handler_) {
                focus_handler_(state_.focused);
            }
        }
    }
    
    [[nodiscard]] const ElementState& GetState() const noexcept { return state_; }
    
    // Trigger actions
    void TriggerClick() {
        if (state_.enabled && click_handler_) {
            click_handler_();
        }
    }

private:
    struct Position {
        float x = 0.0f;
        float y = 0.0f;
    } position_;
    
    struct Size {
        float width = 0.0f;
        float height = 0.0f;
    } size_;
    
    ElementState state_;
    
    ClickHandler click_handler_;
    HoverHandler hover_handler_;
    FocusHandler focus_handler_;
    
    // Only Container can manage element states
    friend class Container;
};

}  // namespace eerium::ui