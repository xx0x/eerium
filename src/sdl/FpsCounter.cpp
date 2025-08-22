#include "FpsCounter.hpp"

#include <algorithm>
#include <format>

#include "sdl/Color.hpp"
#include "sdl/ResourceManager.hpp"

namespace eerium::sdl
{

FpsCounter::FpsCounter()
    : cached_fps_(0.0f),
      fps_dirty_(true),
      last_update_time_(std::chrono::high_resolution_clock::now())
{
}

float FpsCounter::GetFps() const
{
    if (fps_dirty_ && frame_times_.size() >= 2)
    {
        // Calculate FPS based on frame times
        auto duration = frame_times_.back() - frame_times_.front();
        auto duration_seconds = std::chrono::duration<float>(duration).count();

        if (duration_seconds > 0.0f)
        {
            // Number of frame intervals is one less than number of frame times
            cached_fps_ = static_cast<float>(frame_times_.size() - 1) / duration_seconds;
        }
        else
        {
            cached_fps_ = 0.0f;
        }

        fps_dirty_ = false;
    }
    else if (frame_times_.size() < 2)
    {
        cached_fps_ = 0.0f;
    }

    return cached_fps_;
}

void FpsCounter::Render(sdl::Renderer& renderer)
{
    auto now = std::chrono::high_resolution_clock::now();

    // Add current frame time
    frame_times_.push_back(now);

    // Remove old frame times (older than 1 second)
    auto cutoff_time = now - std::chrono::seconds(1);
    while (!frame_times_.empty() && frame_times_.front() < cutoff_time)
    {
        frame_times_.pop_front();
    }

    // Limit the frame history size
    if (frame_times_.size() > kMaxFrameHistory)
    {
        frame_times_.pop_front();
    }

    // Mark FPS as dirty for recalculation
    fps_dirty_ = true;

    // Get the default font from resource manager
    if (!font_.has_value())
    {
        font_ = ResourceManager::Instance().GetDefaultFont();
        if (!font_.has_value())
        {
            return;  // Can't render without font
        }
    }

    // Format FPS text
    float fps = GetFps();
    std::string fps_text = std::format("FPS: {:.1f}", fps);

    // Get window size to position in top right
    auto window_size = renderer.GetWindowSize();

    // Render text in top right corner with some padding
    constexpr float kPadding = 10.0f;
    float x = window_size.width - kPadding - 150;
    float y = kPadding;

    renderer.RenderText(fps_text, x, y, kColorYellow, font_.value(),
                        Renderer::TextAlign::LEFT);
}

}  // namespace eerium::sdl
