#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <functional>

constexpr float PI = 3.14159265358979323846f;

int segmentsAmount = 100;
float segmentsThickness = 0.01f;

glm::vec2 bernstein2(const glm::vec2& p0,const glm::vec2& p1,const glm::vec2& p2,float t) {
    float u = 1 - t;
    return p0 * (u * u) + p1 * (2 * u * t) + p2 * (t * t);
}

void draw_parametric(std::function<glm::vec2(float)> const& parametric) {
    glm::vec2 prev = parametric(0.0f);
    for(int i = 1; i <= segmentsAmount; ++i){
        float t = float(i) / segmentsAmount;
        glm::vec2 cur = parametric(t);
        utils::draw_line(prev,cur,segmentsThickness,glm::vec4(1.0f));
        prev = cur;
    }
}

struct Particle{
    glm::vec2 position;
    glm::vec2 velocity;
    float     radius = 0.005f;
    glm::vec4 color  = glm::vec4(1.0f,0.0f,0.0f,1.0f);
};

int main(){
    gl::init("Particules!");
    gl::maximize_window();

    std::vector<Particle> particles;
    particles.resize(100);

    for(size_t i = 0; i < particles.size(); ++i) {
        particles[i].position = glm::vec2(utils::rand(-1.0f,1.0f),1.0f);
        particles[i].velocity = glm::vec2(0.0f,0.0f);
    }

    while(gl::window_is_open()) {
        float dt = gl::delta_time_in_seconds();
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::vec2 q0(-0.8f,0.0f),q1(gl::mouse_position()),q2(0.8f,0.0f);

        draw_parametric([&](float t){return bernstein2(q0,q1,q2,t);});

        for(size_t i = 0; i < particles.size(); ++i){
            auto& p = particles[i];
            p.velocity += glm::vec2(0.0f,-0.5f) * dt;

            float t = 0.5f;
            for(int iter = 0; iter < 10; ++iter){
                glm::vec2 C = bernstein2(q0,q1,q2,t);
                glm::vec2 dP = 2.0f * (1 - t) * (q1 - q0) + 2.0f * t * (q2 - q1);
                float grad = 2.0f * glm::dot(C - p.position,dP);
                t -= 0.01f * grad;
                t = glm::clamp(t,0.0f,1.0f);
            }

            glm::vec2 Cmin = bernstein2(q0,q1,q2,t);
            glm::vec2 dPmin = 2.0f * (1 - t) * (q1 - q0) + 2.0f * t * (q2 - q1);
            glm::vec2 tangent = glm::normalize(dPmin);
            glm::vec2 normal = glm::vec2(-tangent.y, tangent.x);
            float dist = glm::length(p.position - Cmin);
            float forceMag = std::exp(-dist * 5.0f) * 5.0f;
            glm::vec2 force = normal * forceMag;

            p.velocity += force * dt;
            p.position += p.velocity * dt;

            utils::draw_disk(p.position,p.radius,p.color);
        }
    }
}