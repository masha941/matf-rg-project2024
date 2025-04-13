#include <engine/graphics/BloomEffectController.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/ResourcesController.hpp>

#include "MainController.hpp"

#include <GuiController.hpp>
#include <random>
#include <spdlog/spdlog.h>

#include "../../engine/libs/glad/include/glad/glad.h"

namespace engine::test::app {
    class GUIController;
}
namespace app {

    class MainPlatformEventObserver : public engine::platform::PlatformEventObserver {
    public:
        void on_mouse_move(engine::platform::MousePosition position) override;
    };

    void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (!gui_controller->is_enabled()) {
            auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
            camera->rotate_camera(position.dx, position.dy);
        }
    }

    void MainController::initialize() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto bloom    = engine::core::Controller::get<engine::graphics::BloomEffectController>();
        platform->register_platform_event_observer(std::make_unique<MainPlatformEventObserver>());
        engine::graphics::OpenGL::enable_depth_testing();
        bloom->hdr_bloom_setup();
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::update_camera() {
        auto gui_controller = engine::core::Controller::get<GUIController>();
        if (gui_controller->is_enabled())
            return;
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera   = graphics->camera();
        float dt      = platform->dt();

        if (platform->key(engine::platform::KeyId::KEY_W).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_S).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_A).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_D).is_down()) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
        }
        if (platform->key(engine::platform::KeyId::KEY_LEFT_SHIFT).is_down()) {
            camera->MovementSpeed = 15.0f;
        } else if (platform->key(engine::platform::KeyId::KEY_LEFT_SHIFT).is_up()) {
            camera->MovementSpeed = 10.0f;
        }
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::draw_terrain() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera                       = graphics->camera();
        engine::resources::Model *terrain = resources->model("terrain");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.4f);

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_vec3("viewPos", camera->Position);
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(20.0f, -15.0f, 0.0f));
        model           = glm::scale(model, glm::vec3(3.0f));
        shader->set_mat4("model", model);

        terrain->draw(shader);
    }

    void MainController::draw_gazebo() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera    = graphics->camera();

        engine::resources::Model *gazebo  = resources->model("gazebo");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.1f);

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_vec3("viewPos", camera->Position);
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        shader->set_mat4("model", glm::mat4(1.0f));
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(20.0f, -15.0f, 0.0f));
        float angle     = glm::radians(270.0);
        model           = glm::rotate(model, angle, glm::vec3(0, 1, 0));
        model           = glm::scale(model, glm::vec3(0.04f));
        shader->set_mat4("model", model);

        gazebo->draw(shader);
    }

    void MainController::draw_columns() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera    = graphics->camera();

        engine::resources::Model *column  = resources->model("column");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("viewPos", camera->Position);
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.3f);

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_mat4("projection", graphics->projection_matrix());

        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);

        glm::mat4 model1 = glm::mat4(1.0f);
        glm::mat4 model2 = glm::mat4(1.0f);
        glm::mat4 model3 = glm::mat4(1.0f);
        glm::mat4 model4 = glm::mat4(1.0f);

        model1 = glm::translate(model1, glm::vec3(6.5f, -15.0f, 13.0f)); // front left

        model2 = glm::translate(model2, glm::vec3(34.5f, -15.0f, 13.0f)); // back left

        model3 = glm::translate(model3, glm::vec3(6.5f, -15.0f, -12.5f)); // front right

        model4 = glm::translate(model4, glm::vec3(34.5f, -15.0f, -12.5f)); // back right

        for (auto &model: {model1, model2, model3, model4}) {
            shader->set_mat4("model", model);
            column->draw(shader);
        }
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::draw_skybox() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto skybox    = resources->skybox("sky");
        auto shader    = resources->shader("skybox");
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();
        graphics->draw_skybox(shader, skybox);
    }

    void MainController::draw_butterfly_instanced() {
        auto resources                      = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                       = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto platform                       = engine::core::Controller::get<engine::platform::PlatformController>();
        auto current_time                   = platform->frame_time().current;
        engine::resources::Model *butterfly = resources->model("butterfly");
        engine::resources::Shader *shader   = resources->shader("instancing");
        auto camera                         = graphics->camera();

        unsigned int amount = 200;
        glm::mat4 *modelMatrices;
        modelMatrices       = new glm::mat4[amount];
        float offset        = 1.5f;
        glm::vec3 gazeboPos = glm::vec3(20.0f, -3.0f, 0.0f);

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist(-offset, offset);

        for (unsigned int i = 0; i < amount; i++) {
            glm::mat4 model    = glm::mat4(1.0f);
            float angle        = (float) i / (float) amount * 360.0f;
            float displacement = dist(mt);
            float x            = sin(angle) * m_radius + displacement;
            displacement       = dist(mt);
            float y            = displacement * 0.4f;
            displacement       = dist(mt);
            float z            = cos(angle) * m_radius + displacement;
            model              = glm::translate(model, gazeboPos + glm::vec3(x, y, z));

            float scale = 0.5f;
            model       = glm::scale(model, glm::vec3(scale));

            std::uniform_real_distribution<float> angle_dist(0.0f, 360.0f);
            float rotAngle = angle_dist(mt);
            model          = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.4f, 0.6f, 0.8f));

            modelMatrices[i] = model;
        }

        butterfly->prepare_instance_matrices(amount, modelMatrices);

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(7.0f, 30.0f, -1.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_vec3("viewPos", camera->Position);
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);

        butterfly->draw_instanced(shader, amount);
        delete[] modelMatrices;
    }
    void MainController::draw_garden() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera                       = graphics->camera();
        engine::resources::Model *garden  = resources->model("garden");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.01f);

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_vec3("viewPos", camera->Position);

        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        float angle     = glm::radians(270.0);
        float angle2    = glm::radians(180.0);
        model           = glm::translate(model, glm::vec3(90.0f, -38.0f, 90.0f));
        model           = glm::rotate(model, angle, glm::vec3(1, 0, 0));
        model           = glm::scale(model, glm::vec3(3.0f));
        shader->set_mat4("model", model);

        garden->draw(shader);
    }

    void MainController::draw_statue() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera                       = graphics->camera();
        engine::resources::Model *statue  = resources->model("statue");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.3f);

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_vec3("viewPos", camera->Position);
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        float angle     = glm::radians(270.0);
        float angle2    = glm::radians(90.0);
        model           = glm::translate(model, glm::vec3(20.0f, -14.0f, 0.0f));
        model           = glm::rotate(model, angle, glm::vec3(0, 1, 0));
        model           = glm::rotate(model, angle, glm::vec3(1, 0, 0));
        model           = glm::scale(model, glm::vec3(0.5f));
        shader->set_mat4("model", model);

        statue->draw(shader);
    }

    void MainController::draw_temple() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera                       = graphics->camera();
        engine::resources::Model *temple  = resources->model("temple");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.4f);

        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_vec3("viewPos", camera->Position);
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(70.0f, -15.5f, -30.0f));
        model           = glm::scale(model, glm::vec3(2.0f));
        shader->set_mat4("model", model);

        temple->draw(shader);
    }

    void MainController::draw_tree() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera                       = graphics->camera();
        engine::resources::Model *tree    = resources->model("tree");
        engine::resources::Shader *shader = resources->shader("basic");

        shader->use();
        shader->set_vec3("LightPos", glm::vec3(21.0f, -5.0f, 2.0f));
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_float("specularStrength", 0.0f);
        for (int i = 0; i < 5; i++) {
            shader->set_vec3("LampPos[" + std::to_string(i) + "]", light.get_lamp_positions()[i]);
            shader->set_vec3("LampColor[" + std::to_string(i) + "]", light.get_lamp_colors()[i]);
        }

        shader->set_vec3("moonLightDir", glm::vec3(-25.0f, 30.0f, 0.0f));
        shader->set_vec3("moonLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_vec3("viewPos", camera->Position);
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(20.0f, -15.0f, -30.0f));
        model           = glm::scale(model, glm::vec3(1.0f));
        shader->set_mat4("model", model);

        tree->draw(shader);
    }

    void MainController::draw_light() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *light   = resources->model("light");
        engine::resources::Shader *shader = resources->shader("light");

        shader->use();
        shader->set_vec3("LightColor", glm::vec3(m_light_gazebo, m_light_gazebo, m_light_gazebo));
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, glm::vec3(21.0f, -5.0f, 2.0f));
        model           = glm::scale(model, glm::vec3(7.0f));
        shader->set_mat4("model", model);

        light->draw(shader);
    }

    void MainController::draw_street_lamp() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                     = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera                       = graphics->camera();
        engine::resources::Model *lamp    = resources->model("street_lamp");
        engine::resources::Shader *shader = resources->shader("light");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        glm::mat4 view = graphics->camera()->view_matrix();
        view           = glm::scale(view, glm::vec3(0.3f));
        shader->set_mat4("view", view);

        glm::mat4 model2 = glm::mat4(1.0f);
        glm::mat4 model3 = glm::mat4(1.0f);
        glm::mat4 model4 = glm::mat4(1.0f);
        glm::mat4 model5 = glm::mat4(1.0f);

        model2 = glm::translate(model2, glm::vec3(90.0f, -10.0f, -20.0f));
        model2 = glm::scale(model2, glm::vec3(2.5f));

        model3 = glm::translate(model3, glm::vec3(55.0f, -10.0f, -20.0f));
        model3 = glm::scale(model3, glm::vec3(2.5f));

        model4 = glm::translate(model4, glm::vec3(90.0f, -10.0f, -40.5f));
        model4 = glm::scale(model4, glm::vec3(2.5f));

        model5 = glm::translate(model5, glm::vec3(55.0f, -10.0f, -40.5f));
        model5 = glm::scale(model5, glm::vec3(2.5f));

        glm::mat4 model1 = glm::mat4(1.0f);
        model1           = glm::translate(model1, glm::vec3(20.0f, -15.0f, -23.0f));
        model1           = glm::scale(model1, glm::vec3(2.5f));
        shader->set_mat4("model", model1);

        int i = 0;
        for (auto &model: {model1, model2, model3, model4, model5}) {
            shader->set_vec3("LightColor", light.get_lamp_colors()[i]);
            shader->set_mat4("model", model);
            lamp->draw(shader);
            i++;
        }
    }

    void MainController::draw_butterflies_delayed() {
        auto platform                = engine::core::Controller::get<engine::platform::PlatformController>();
        auto current_time            = platform->frame_time().current;
        static double butterflyDelay = -1.0;
        double delay                 = 3.0;
        if (m_light_gazebo > 50.0f) {
            draw_statue();
            if (butterflyDelay < 0)
                butterflyDelay = current_time;

            if (current_time - butterflyDelay >= delay)
                draw_butterfly_instanced();
        } else
            butterflyDelay = -1.0;
    }

    void MainController::draw() {
        auto bloom = engine::core::Controller::get<engine::graphics::BloomEffectController>();
        bloom->prepare_hdr();

        draw_garden();
        draw_terrain();
        draw_street_lamp();
        draw_gazebo();
        draw_light();
        draw_temple();
        draw_butterflies_delayed();
        draw_columns();
        draw_tree();
        draw_skybox();

        bloom->finalize_bloom();
    }
} // namespace app
