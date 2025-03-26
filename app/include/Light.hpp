#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

namespace app {

    class Light {
    public:
        void set_lamp_colors(int index, const glm::vec3 &color) {
            if (index >= 0 && index < 5) {
                m_lampColors[index] = color;
            }
        }
        void set_base_colors(int index, const glm::vec3 &color) {
            if (index >= 0 && index < 5) {
                m_baseColors[index] = color;
            }
        }
        const glm::vec3 *get_lamp_colors() const {
            return m_lampColors;
        }
        glm::vec3 *get_lamp_positions() {
            return m_lampPositions;
        }
        glm::vec3 *get_base_colors() {
            return m_baseColors;
        }
        float *get_brightness() {
            return m_brightness;
        }

    private:
        glm::vec3 m_lampColors[5] = {
                glm::vec3(30.0f, 5.0f, 25.0f), // pink
                glm::vec3(30.0f, 30.0f, 5.0f), // yellow
                glm::vec3(5.0f, 5.0f, 30.0f),  // blue
                glm::vec3(5.0f, 30.0f, 5.0f),  // green
                glm::vec3(20.0f, 10.0f, 10.0f) // red
        };
        glm::vec3 m_lampPositions[5] = {glm::vec3(20.0f, -2.0f, -23.0f), glm::vec3(90.0f, 2.0f, -20.0f),
                                        glm::vec3(55.0f, 2.0f, -20.0f), glm::vec3(90.0f, 2.0f, -40.5f),
                                        glm::vec3(55.0f, 2.0f, -40.5f)};
        glm::vec3 m_baseColors[5];
        float m_brightness[5];
    };
} // namespace app

#endif // LIGHT_HPP
