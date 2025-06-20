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
    glm::vec4 startColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 endColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

struct Wall {
    glm::vec2 A;
    glm::vec2 B;
};

struct Circle {
    glm::vec2 center;
    float radius;
};

int main() {
    gl::init("Particules!");
    gl::maximize_window();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // TODO: create an array of particles

    std::vector<Particle> particles;
    for (size_t i = 0; i < 10; i++) {
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
        particle.startColor = glm::vec4(utils::rand(0.0f, 1.0f), utils::rand(0.0f, 1.0f), utils::rand(0.0f, 1.0f), 1.0f);
        particle.endColor = glm::vec4(utils::rand(0.0f, 1.0f), utils::rand(0.0f, 1.0f), utils::rand(0.0f, 1.0f), 1.0f);
        particles.push_back(particle);
    }

    std::vector<Wall> walls;
    for (size_t i = 0; i < 4; i++) {
        Wall wall;

        if (i == 0) {
            wall.A = glm::vec2(-1.0f, -1.0f);
            wall.B = glm::vec2( 1.0f, -1.0f);
        } else if (i == 1) {
            wall.A = glm::vec2( 1.0f, -1.0f);
            wall.B = glm::vec2( 1.0f,  1.0f);
        } else if (i == 2) {
            wall.A = glm::vec2( 1.0f,  1.0f);
            wall.B = glm::vec2(-1.0f,  1.0f);
        } else {
            wall.A = glm::vec2(-1.0f,  1.0f);
            wall.B = glm::vec2(-1.0f, -1.0f);
        }

        walls.push_back(wall);
    }

    std::vector<Circle> circles;
    for (size_t i = 0; i < 3; i++) {
        Circle circle;
        circle.center = glm::vec2(utils::rand(-1.0f, 1.0f), utils::rand(-1.0f, 1.0f));
        circle.radius = utils::rand(0.05f, 0.15f);
        circles.push_back(circle);
    }

    float airDynamicViscosity = 4.f;

    while (gl::window_is_open()) {
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO update particles

        // TODO render particles

        // Second wall
        glm::vec2 C( 0.0f,-1.0f);
        glm::vec2 D( gl::mouse_position());
        utils::draw_line(C, D, 0.01f, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

        for (const Wall& wall : walls) {
            utils::draw_line(wall.A, wall.B, 0.01f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

            // Cross check
            glm::vec2 r = wall.B - wall.A;
            glm::vec2 s = D - C;

            glm::mat2 M(r, -s);    

            glm::vec2 v = C - wall.A;

            float det = glm::determinant(M);

            if (fabs(det) > 1e-6f) {
                glm::vec2 t = glm::inverse(M) * v;

                float u = t.x;
                float v = t.y;

                if (u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f) {
                    // crossing point
                    glm::vec2 P = wall.A + u * r;
                    utils::draw_disk(P, 0.05f, glm::vec4(0.0f,1.0f,0.0f,1.0f));
                }
            }
        }

        for (const Circle& circle : circles) {
            utils::draw_disk(circle.center, circle.radius, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        if (particles.size() == 0) {
            return 0;
        }

        for (size_t i = 0; i < particles.size(); i++) {
            //particles[i].age += gl::delta_time_in_seconds();

            // float t = glm::clamp(particles[i].age / particles[i].lifetime, 0.0f, 1.0f);
            // particles[i].radius = particles[i].initialRadius * (1.0f - t);

            float t = glm::clamp((particles[i].lifetime - particles[i].age) / particles[i].ageOfDecreasing, 0.0f, 1.0f);
            //particles[i].radius = particles[i].initialRadius * t;

            //particles[i].color = glm::mix(particles[i].startColor, particles[i].endColor, t);

            float p = 2.0f;
            float t_ease = std::pow(t, p);
            particles[i].color = glm::mix(particles[i].startColor, particles[i].endColor, t_ease);

            if (particles[i].age / particles[i].lifetime >= 1.0f) {
                particles.erase(particles.begin() + i);
                continue;
            }

            // glm::vec2 normalizedDirection = glm::normalize(particles[i].direction);

            // particles[i].direction = normalizedDirection;
            // particles[i].speed = -3.0f * 3.14f * airDynamicViscosity * particles[i].radius * particles[i].speed;
            
            glm::vec2 forces(0.f, 0.f);
            //forces += particles[i].mass * 9.81f * glm::vec2(0.f, -1.f); // Gravity
            //forces +=  - airDynamicViscosity * particles[i].radius * particles[i].velocity; //Friction
            //forces += particles[i].radius * particles[i].velocity; //Friction
            //forces += gl::mouse_position();

            particles[i].velocity += forces * gl::delta_time_in_seconds() / particles[i].mass;
            glm::vec2 prevPos = particles[i].position;
            particles[i].position += particles[i].velocity * gl::delta_time_in_seconds();
            glm::vec2 currPos = particles[i].position;

            for (const Wall& wall : walls) {
                glm::vec2 particlePosition( particles[i].position.x, particles[i].position.y);

                // Cross check
                glm::vec2 r = wall.B - wall.A;
                glm::vec2 s = particlePosition - (particlePosition * 1.1f);

                glm::mat2 M(r, -s);    

                glm::vec2 v = particlePosition - wall.A;

                float det = glm::determinant(M);

                if (fabs(det) > 1e-6f) {
                    glm::vec2 t = glm::inverse(M) * v;

                    float u = t.x;
                    float v = t.y;

                    if (u >= 0.0f && u <= 1.0f && v >= 0.0f && v <= 1.0f) {
                        // crossing point
                        glm::vec2 P = wall.A + u * r;
                        glm::vec2 normal = glm::normalize(glm::vec2(-r.y, r.x)); // Perpendicular to the wall
                        particles[i].velocity = glm::reflect(particles[i].velocity, normal);
                        particles[i].position = P + glm::normalize(particles[i].velocity) * glm::distance(P, currPos) ;
                    }
                }
            }

            for (const Circle& circle : circles) {
                glm::vec2 particlePosition( particles[i].position.x, particles[i].position.y);
                float distance = glm::distance(particlePosition, circle.center);

                if (distance < circle.radius + particles[i].radius) {
                    glm::vec2 normal = glm::normalize(particlePosition - circle.center);
                    particles[i].velocity = glm::reflect(particles[i].velocity, normal);
                    particles[i].position = circle.center + normal * (circle.radius + particles[i].radius);
                }
            }

            //Draw the particle
            utils::draw_disk(particles[i].position, particles[i].radius, particles[i].color);
        }
    }
}