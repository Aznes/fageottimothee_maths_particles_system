#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <functional>

constexpr float PI = 3.14159265358979323846f;

int segmentsAmount = 100;
float segmentsThickness = 0.01f;

glm::vec2 bernstein3(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, float t) {
    float u = 1 - t;
    return p0 * (u * u * u) + p1 * (3 * u * u * t) + p2 * (3 * u * t * t) + p3 * (t * t * t);
}

void draw_parametric(std::function<glm::vec2(float)> const& parametric) {
    glm::vec2 prev = parametric(0.0f);
    
    for(int i = 1; i <= segmentsAmount; ++i) {
        float t       = float(i) / segmentsAmount;
        glm::vec2 cur = parametric(t);
        utils::draw_line(prev, cur, segmentsThickness, glm::vec4(1.0f));
        prev = cur;
    }
}

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius = 0.005f;
    glm::vec4 color  = glm::vec4(1.0f,0.0f,0.0f,1.0f);
    float speed  = 0.5f;
};

int main(){
    gl::init("Particules!");
    gl::maximize_window();

    std::vector<Particle> particles;
    particles.resize(100);

    std::vector<float> times(particles.size());
    for(size_t i = 0; i < times.size(); ++i) {
        times[i] = float(i) / float(times.size() - 1);
    }

    glm::vec2 q0(-0.8f,0.0f), q1(-0.2f, -0.8f), q2(0.8f,0.0f), q3(0.9f, 0.5f);

    for(size_t i = 0; i < particles.size(); ++i) {
        float t = times[i];
        glm::vec2 pos2 = bernstein3(q0,q1,q2, q3,t);
        particles[i].position = pos2;

        glm::vec2 dP = 2.0f * (1.0f - t) * (q1 - q0) + 2.0f * t * (q2 - q1);
        glm::vec2 tangent = glm::normalize(dP);
        glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);
        particles[i].velocity = normal * particles[i].speed;
    }

    while(gl::window_is_open()){
        glClearColor(0.f,0.f,0.f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_parametric([&](float t) { return bernstein3(q0,q1,q2, q3,t); });

        for(size_t i = 0; i < particles.size(); ++i) {
            auto& p = particles[i];
            p.position += p.velocity * gl::delta_time_in_seconds();
            utils::draw_disk(p.position, p.radius, p.color);
        }
    }
}