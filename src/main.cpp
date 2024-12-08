#include <SFML/Graphics.hpp>
#include <random>
#include "events.hpp"
#include "configuration.hpp"
#include "star.hpp"

std::vector<Star> CreateStars(uint32_t count, float scale)
{
    std::vector<Star> stars;
    stars.reserve(count);

    // Random numbers generator
    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Create randomly distributed stars on the screen
    for (uint32_t i{count}; i --;)
    {
        const float x = (dis(gen) - 0.5f) * conf::window_size_f.x * scale;
        const float y = (dis(gen) - 0.5f) * conf::window_size_f.y * scale;
        const float z = dis(gen) * (conf::far_distance - conf::near_distance) + conf::near_distance;
    
        stars.push_back({{x, y}, z});
    }

    // Depth ordering
    sort(stars.begin(), stars.end(), [](const Star &a, const Star &b) { return a.z > b.z; } );

    return stars;
}


void UpdateGeometry(uint32_t idx, const Star &s, sf::VertexArray &va)
{
    const float scale = 1.0f / s.z;
    const float depth_ratio = (s.z - conf::near_distance) / (conf::far_distance - conf::near_distance);
    const float color_ratio = 1.0f - depth_ratio;
    uint32_t const c = static_cast<uint32_t>(color_ratio * 255.0f);
   
    const sf::Vector2f p = s.position * scale;
    const float r = conf::star_radius * scale;
    const uint32_t i = 4 * idx;

    va[i + 0].position = {p.x - r, p.y - r};
    va[i + 1].position = {p.x + r, p.y - r};
    va[i + 2].position = {p.x + r, p.y + r};
    va[i + 3].position = {p.x - r, p.y + r};

    const sf::Color color(c, c, c);
    va[i + 0].color = va[i + 1].color = va[i + 2].color = va[i + 3].color = color;
}

int main()
{
    sf::RenderWindow window({conf::window_size.x, conf::window_size.y}, "Lets go", sf::Style::Fullscreen);
    window.setFramerateLimit(conf::max_framerate);
    window.setMouseCursorVisible(false);

    sf::Texture texture;
    texture.loadFromFile("../img/star4.png");
    texture.setSmooth(true);
    texture.generateMipmap();

    std::vector<Star> stars = CreateStars(conf::star_count, conf::far_distance);

    sf::VertexArray va{sf::PrimitiveType::Quads, 4 * conf::star_count};
    
    // Pre fill texture coords as they will remain constant
    const sf::Vector2f texture_size_f = static_cast<sf::Vector2f>(texture.getSize());
    for (uint32_t idx{conf::star_count}; idx --;)
    {
        const uint32_t i = 4 * idx;
        va[i + 0].texCoords = {0.0f, 0.0f};
        va[i + 1].texCoords = {texture_size_f.x, 0.0f};
        va[i + 2].texCoords = {texture_size_f.x, texture_size_f.y};
        va[i + 3].texCoords = {0.0f, texture_size_f.y};
    }

    while (window.isOpen())
    {
        ProcessEvents(window);

        // Fake travel towards inсreasing Z
        std::vector<Star> next_stars;
        for (Star &s : stars)
        {
            s.z -= conf::dt * conf::star_speed;
            if (s.z < conf::near_distance)
            {
                s.z += conf::far_distance - conf::near_distance;
                next_stars.push_back(s);
            }
        }
        int add_index = 0;
        while (next_stars.size() < conf::star_count) next_stars.push_back(stars[add_index ++]);
        stars = next_stars;


        // Rendering occurs here
        window.clear();

        sf::CircleShape shape{conf::star_radius};
        shape.setOrigin(conf::star_radius, conf::star_radius);
        
        for (uint32_t i{0}; i < conf::star_count; i ++)
        {
            const Star &s = stars[i];
            UpdateGeometry(i, s, va);
        }

        sf::RenderStates states;
        states.transform.translate(conf::window_size_f * 0.5f);
        states.texture = &texture;
        window.draw(va, states);

        window.display();
    }

    return 0;
}
