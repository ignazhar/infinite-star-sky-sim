#pragma once

#include <SFML/System/Vector2.hpp>

namespace conf
{
    // Window configuration
    const sf::Vector2u window_size = {1920, 1280};
    const sf::Vector2f window_size_f = static_cast<sf::Vector2f>(window_size);
    const unsigned int max_framerate = 60;
    const float dt = 1.0f / static_cast<float>(max_framerate);
    
    // Star configuration
    const unsigned int star_count = 200000;
    const float star_radius = 20.0f;
    const float star_speed = 1.0f;
    const float near_distance = 0.01f;
    const float far_distance = 10.0f;
}