#include "opengl-framework/opengl-framework.hpp"
#include "utils.hpp"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>

struct Particle {
    glm::vec2 position;
    glm::vec2 velocity;
    float     radius = 0.005f;
    glm::vec4 color  = glm::vec4(1.0f);
};

int main() {
    gl::init("Particules!");
    gl::maximize_window();

    constexpr float PI = 3.14159265358979323846f;
    constexpr float R = 0.3f;
    constexpr float minDist = 0.02f;
    constexpr int k = 30;

    float cellSize = minDist / std::sqrt(2.0f);
    int gridW = static_cast<int>(std::ceil(2 * R / cellSize));
    int gridH = gridW;

    glm::vec2 origin(0.0f, 0.0f);
    glm::vec2 xEnd  (0.0f, 1.0f);
    glm::vec2 yEnd  (1.0f, 0.0f);
    glm::vec2 bx = xEnd - origin;
    glm::vec2 by = yEnd - origin;
    glm::vec2 center = origin + (bx + by) * 0.5f;

    std::vector<int> grid(gridW * gridH, -1);
    std::vector<glm::vec2> samples;
    std::vector<int> active;

    auto toGrid = [&](const glm::vec2& p) {
        int gridX = static_cast<int>((p.x + R) / cellSize);
        int gridY = static_cast<int>((p.y + R) / cellSize);

        return std::make_pair(gridX, gridY);
    };

    glm::vec2 position0;

    do {
        float x = utils::rand(-R, R);
        float y = utils::rand(-R, R);
        position0 = glm::vec2(x, y);
    } while (glm::length(position0) > R);

    samples.push_back(position0);
    active.push_back(0);
    auto[gridX0, gridY0] = toGrid(position0);
    grid[gridY0 * gridW + gridX0] = 0;

    while (!active.empty()) {
        int randomIndex = static_cast<int>(utils::rand(0.0f, (float)active.size()));

        if (randomIndex == active.size()) randomIndex--;

        int index = active[randomIndex];
        bool found = false;

        for (int i = 0; i < k; i++) {
            float randomRadius = utils::rand(minDist, 2 * minDist);
            float theta = utils::rand(0.0f, 1.0f) * 2.0f * PI;
            glm::vec2 cand = samples[index] + glm::vec2(randomRadius * std::cos(theta), randomRadius * std::sin(theta));

            if (glm::length(cand) > R) continue;

            auto[gridX, gridY] = toGrid(cand);

            if (gridX < 0 || gridX >= gridW || gridY < 0 || gridY >= gridH) continue;

            bool ok = true;

            for (int ix = std::max(0, gridX-2); ix <= std::min(gridW-1, gridX+2); ix++) {
                for (int iy = std::max(0, gridY-2); iy <= std::min(gridH-1, gridY+2); iy++) {
                    int sidx = grid[iy * gridW + ix];

                    if (sidx != -1 && glm::length(cand - samples[sidx]) < minDist) {
                        ok = false;
                        break;
                    }
                }
            }

            if (!ok) continue;

            samples.push_back(cand);
            active.push_back(static_cast<int>(samples.size()) - 1);
            grid[gridY * gridW + gridX] = static_cast<int>(samples.size()) - 1;
            found = true;

            break;
        }

        if (!found) {
            active.erase(active.begin() + randomIndex);
        }
    }

    std::vector<Particle> particles;

    particles.reserve(samples.size());

    for (auto& localPos : samples) {
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