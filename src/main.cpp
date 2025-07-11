#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius = 0.005f;
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
};


int main() {
    gl::init("Particules!");
    gl::maximize_window();

    std::vector<Particle> particles;
    for (size_t i = 0; i < 5000; i++) {
        float minPos = 0.0f;
        float maxPos = 1.0f;

        Particle particle;
        particle.position = glm::vec2(utils::rand(minPos, maxPos), utils::rand(minPos, maxPos));
        particles.push_back(particle);
    }

    glm::vec2 origin(0.0f, 0.0f);
    glm::vec2 xEnd  (1.0f, 0.0f);
    glm::vec2 yEnd  (0.0f, 1.0f);
    glm::vec2 bx = xEnd - origin;
    glm::vec2 by = yEnd - origin;

    while (gl::window_is_open()) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        utils::draw_line(origin, xEnd, 0.005f, glm::vec4(1,0,0,1));
        utils::draw_line(origin, yEnd, 0.005f, glm::vec4(0,1,0,1));

        for (size_t i = 0; i < particles.size(); i++) {
            glm::vec2 worldPos = origin + bx * particles[i].position.x + by * particles[i].position.y;
            utils::draw_disk(worldPos, particles[i].radius, particles[i].color);
        }
    }
}