#pragma once

#include <chrono>
#include <deque>
#include <optional>

#include "sdl/Renderer.hpp"

namespace eerium::sdl
{
class FpsCounter
{
public:
    FpsCounter();

    /**
     * @brief Get the current FPS
     * @return Current FPS as a float
     */
    float GetFps() const;

    /**
     * @brief Render the FPS counter in the top right corner
     * @param renderer The renderer to use for drawing
     */
    void Render(sdl::Renderer& renderer);

private:
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    std::deque<TimePoint> frame_times_;
    mutable float cached_fps_;
    mutable bool fps_dirty_;

    std::optional<Font> font_;

    static constexpr size_t kMaxFrameHistory = 60;     // Track last 60 frames
    static constexpr float kMinUpdateInterval = 1.0f;  // Update FPS display every 1 second

    TimePoint last_update_time_;
};
}  // namespace eerium::sdl