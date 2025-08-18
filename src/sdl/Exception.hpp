#pragma once
#include <stdexcept>
#include <string>

namespace eerium::sdl
{

/**
 * @brief Exception thrown when SDL initialization fails
 */
class Exception : public std::runtime_error
{
public:
    explicit Exception(const std::string& message)
        : std::runtime_error(message) {}
};
}  // namespace eerium::sdl