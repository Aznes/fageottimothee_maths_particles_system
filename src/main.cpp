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

void draw_parametric(std::function<glm::vec2(float)> const& parametric)
{
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

    glm::vec2 l0(-0.8f, 0.8f), l1(0.8f, 0.8f);

    glm::vec2 q0(-0.8f,  0.0f), q1(0.0f, 0.8f), q2(0.8f, 0.0f);

    glm::vec2 c0(-0.8f, -0.8f), c1(-0.4f, 0.8f), c2(0.4f, -0.8f), c3(0.8f, -0.2f);

    while (gl::window_is_open()) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        draw_parametric([&](float t) { return bernstein1(l0, l1, t); });
        draw_parametric([&](float t) { return bernstein2(q0, q1, q2, t); });
        draw_parametric([&](float t) { return bernstein3(c0, c1, c2, c3, t); });
    }
}