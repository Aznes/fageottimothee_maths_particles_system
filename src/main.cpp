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

glm::vec2 bezier3(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec2 p = uuu * p0;
    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;   
    p += ttt * p3;

    return p;
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

    while (gl::window_is_open()) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        draw_parametric([](float t) {
            float angle = 2.0f * PI * t;
            float x = 16.0f * std::pow(std::sin(angle), 3);
            float y = 13.0f * std::cos(angle)
                    -  5.0f * std::cos(2.0f * angle)
                    -  2.0f * std::cos(3.0f * angle)
                    -      std::cos(4.0f * angle);
            glm::vec2 p = glm::vec2(x, y) * (circleRadius / 16.0f);
            return circleCenterPosition + p;
        });
    }
}