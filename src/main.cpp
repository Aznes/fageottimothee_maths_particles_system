#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

constexpr float PI = 3.14159265358979323846f;

glm::vec2 circleCenterPosition  = glm::vec2(0.0f, 0.0f);
float circleRadius  = 0.5f;

int segmentsAmount = 100;
float segmentsThickness = 0.01f;

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float     radius = 0.005f;
    glm::vec4 color  = glm::vec4(1.0f,0.0f,0.0f,1.0f);
};

glm::vec2 bernstein1(const glm::vec2& p0, const glm::vec2& p1, float t) {
    return (1 - t) * p0 + t * p1;
}

glm::vec2 bernstein2(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, float t) {
    float u = 1 - t;
    return p0 * (u * u) + p1 * (2 * u * t) + p2 * (t * t);
}

glm::vec2 bernstein3(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, float t) {
    float u = 1 - t;
    return p0 * (u * u * u) + p1 * (3 * u * u * t) + p2 * (3 * u * t * t) + p3 * (t * t * t);
}

void draw_parametric(std::function<glm::vec2(float)> const& parametric){
    glm::vec2 prev = parametric(0.0f);

    for (int i = 1; i <= segmentsAmount; ++i) {
        float t = float(i) / segmentsAmount;
        glm::vec2 cur = parametric(t);
        utils::draw_line(prev, cur, segmentsThickness, glm::vec4(1.0f));
        prev = cur;
    }
}

int main() {
    gl::init("Particules!");
    gl::maximize_window();

    std::vector<Particle> particles;
    for (size_t i = 0; i < 100; i++) {
        Particle particle;
        particles.push_back(particle);
    }

    std::vector<float> times;
    for (size_t i = 0; i < 100; i++) {
        float time = utils::rand(0.0f,1.0f);
        times.push_back(time);
    }

    while (gl::window_is_open()) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glm::vec2 q0(-0.8f,  0.0f), q1(gl::mouse_position()), q2(0.8f, 0.0f);

        draw_parametric([&](float t) { return bernstein2(q0, q1, q2, t); });

        for (size_t i = 0; i < particles.size(); ++i) {
            auto& p = particles[i];
            float t = times[i];
            p.position = glm::vec3(bernstein2(q0, q1, q2, t), 1.0f);
            utils::draw_disk(p.position, p.radius, p.color);
        }
    }
}