#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float radius = 0.05f;
    float initialRadius;
    // float speed;
    float mass;
    float lifetime;
    float age = 0.0f;
    float ageOfDecreasing = 2.0f; // Time after which the radius starts to decrease
};

int main() {
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // TODO: create an array of particles

    std::vector<Particle> particles;
    for (size_t i = 0; i < 100; i++) {
        float minPos = -1.0f;
        float maxPos = 1.0f;
        float minSpeed = 0.1f;
        float maxSpeed = 0.5f;
        float minLifetime = 2.0f;
        float maxLifetime = 5.0f;

        Particle particle;
        particle.position = glm::vec2(utils::rand(minPos, maxPos), utils::rand(minPos, maxPos));
        particle.velocity = glm::vec2(utils::rand(minPos, maxPos), utils::rand(minPos, maxPos));
        particle.lifetime = utils::rand(minLifetime, maxLifetime);
        particle.mass = 0.1f;
        particle.initialRadius = particle.radius;
        particle.ageOfDecreasing = 2.0f;
        particles.push_back(particle);
    }

    float airDynamicViscosity = 4.f;

    while (gl::window_is_open()) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO update particles

        // TODO render particles

        if (particles.size() == 0) {
            return 0;
        }

        for (size_t i = 0; i < particles.size(); i++) {
            particles[i].age += gl::delta_time_in_seconds();

            //particles[i].radius = (1 - (particles[i].age / particles[i].lifetime)) * particles[i].initialRadius;

            // float t = glm::clamp(particles[i].age / particles[i].lifetime, 0.0f, 1.0f);
            // particles[i].radius = particles[i].initialRadius * (1.0f - t);

            float t = glm::clamp((particles[i].lifetime - particles[i].age) / particles[i].ageOfDecreasing,
                     0.0f, 1.0f);
            particles[i].radius = particles[i].initialRadius * t;

            if (particles[i].age / particles[i].lifetime >= 1.0f) {
                particles.erase(particles.begin() + i);
                continue;
            }
            // glm::vec2 normalizedDirection = glm::normalize(particles[i].direction);

            // Check for collision with the window borders
            //particles[i].direction = normalizedDirection;
            // particles[i].speed = -3.0f * 3.14f * airDynamicViscosity * particles[i].radius * particles[i].speed;
            glm::vec2 forces(0.f, 0.f);
            //forces += particles[i].mass * 9.81f * glm::vec2(0.f, -1.f); // Gravity
            forces +=  - airDynamicViscosity * particles[i].radius * particles[i].velocity; //Friction
            forces += gl::mouse_position();

            particles[i].velocity += forces * gl::delta_time_in_seconds() / particles[i].mass;
            particles[i].position += particles[i].velocity * gl::delta_time_in_seconds();

            

            // Check for collision with the window bottom border and tp to the top
            // if(particles[i].position.y < -1.0f) {
            //     particles[i].position.y = 1.0f;
            // }

            // Check for collision with the window borders
            if (particles[i].position.x < -1.0f || particles[i].position.x > 1.0f) {
                particles[i].velocity.x = -particles[i].velocity.x;
                particles[i].position.x = glm::clamp(particles[i].position.x, -1.0f, 1.0f);
            }
            if (particles[i].position.y < -1.0f || particles[i].position.y > 1.0f) {
                particles[i].velocity.y = -particles[i].velocity.y;
                particles[i].position.y = glm::clamp(particles[i].position.y, -1.0f, 1.0f);
            }

            // Update position
            // particles[i].position += particles[i].speed * particles[i].direction * gl::delta_time_in_seconds();

            //Draw the particle
            utils::draw_disk(particles[i].position, particles[i].radius, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }
}
