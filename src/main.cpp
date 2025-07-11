#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float     radius = 0.005f;
    glm::vec4 color  = glm::vec4(1.0f,1.0f,1.0f,1.0f);
};

int main() {
    gl::init("Particules!");
    gl::maximize_window();

    constexpr float R  = 0.3f;

    glm::vec2 origin(0.0f,0.0f);
    glm::vec2 xEnd  (0.0f,1.0f);
    glm::vec2 yEnd  (1.0f,0.0f);
    glm::vec2 bx = xEnd - origin;
    glm::vec2 by = yEnd - origin;
    glm::vec2 center = origin + (bx + by) * 0.5f;

    std::vector<Particle> particles;
    for (size_t i = 0; i < 1000; i++) {
        glm::vec2 localPos;

        do {
            float x = utils::rand(-R, R);
            float y = utils::rand(-R, R);
            localPos = glm::vec2(x, y);
        } while (glm::length(localPos) > R);

        glm::vec2 worldPos = center + bx * localPos.x + by * localPos.y;
        Particle p;
        p.position = worldPos;
        particles.push_back(p);
    }

    while (gl::window_is_open()) {
        glClear(GL_COLOR_BUFFER_BIT);

        utils::draw_line(origin, xEnd, 0.005f, glm::vec4(1,0,0,1));
        utils::draw_line(origin, yEnd, 0.005f, glm::vec4(0,1,0,1));

        for (auto& p : particles) {
            utils::draw_disk(p.position, p.radius, p.color);
        }
    }
}