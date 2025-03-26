#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include "Light.hpp"

namespace app {
    class MainController : public engine::core::Controller {

        void initialize() override;
        bool loop() override;
        void update_camera();
        void update() override;
        void draw_terrain();
        void draw_gazebo();
        void draw_columns();
        void draw_skybox();
        void draw_butterfly_instanced();
        void draw_garden();
        void draw_statue();
        void draw_temple();
        void draw_tree();
        void draw_light();
        void draw_street_lamp();
        void draw_butterflies_delayed();
        void draw() override;
        void begin_draw() override;
        void end_draw() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }
        float get_radius() const {
            return m_radius;
        }
        float get_light_gazebo() const {
            return m_light_gazebo;
        }
        void set_radius(float radius) {
            m_radius = radius;
        }
        void set_light_gazebo(float light) {
            m_light_gazebo = light;
        }
        Light light;

    private:
        float m_radius       = 15.0f;
        float m_light_gazebo = 20.0f;
    };

} // namespace app

#endif // MAINCONTROLLER_HPP
