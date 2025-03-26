#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>
#include "GuiController.hpp"

#include <MainController.hpp>
#include <imgui.h>
#include "Light.hpp"

namespace engine::test::app {
    class MainController;
}
namespace app {
    void GUIController::initialize() {
        set_enable(false);
    }

    void GUIController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_H).state() == engine::platform::Key::State::JustPressed) {
            set_enable(!is_enabled());
            platform->set_enable_cursor(is_enabled());
        }
    }

    void GUIController::updateLampColors() {
        auto mainController = engine::core::Controller::get<app::MainController>();

        for (int i = 0; i < 5; i++) {
            mainController->light.set_lamp_colors(i, mainController->light.get_base_colors()[i] *
                                                             mainController->light.get_brightness()[i]);
        }
    }

    void GUIController::draw() {
        auto graphics       = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto mainController = engine::core::Controller::get<app::MainController>();

        graphics->begin_gui();

        ImGui::Begin("ImGui");
        ImGuiIO &io        = ImGui::GetIO();
        io.FontGlobalScale = 1.2f;

        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors    = style.Colors;

        colors[ImGuiCol_WindowBg] = ImVec4(0.245f, 0.173f, 0.210f, 0.75f);
        colors[ImGuiCol_Button]   = ImVec4(0.173f, 0.216f, 0.230f, 1.0f);

        style.FrameRounding = 4.0f;
        style.FramePadding  = ImVec2(8, 4);

        ImGui::Text("Change the intensity of the light in gazebo and see what happens!!!");
        ImGui::Spacing();
        ImGui::Separator();
        float lightgazeboGUI = mainController->get_light_gazebo();
        ImGui::DragFloat("gazebo light", &lightgazeboGUI, 1, 1.0f, 150.0f, "%f", ImGuiSliderFlags_AlwaysClamp);
        mainController->set_light_gazebo(lightgazeboGUI);

        ImGui::Spacing();
        ImGui::Separator();
        if (lightgazeboGUI > 50.0f) {
            float radiusGUI = mainController->get_radius();
            ImGui::DragFloat("radius of butterflies", &radiusGUI, 1, 1.0f, 50.0f, "%f", ImGuiSliderFlags_AlwaysClamp);
            mainController->set_radius(radiusGUI);
        }
        ImGui::Spacing();
        ImGui::Separator();

        static bool alpha_preview      = true;
        static bool alpha_half_preview = false;
        static bool drag_and_drop      = true;
        static bool options_menu       = true;
        static bool hdr                = false;

        ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) |
                                         (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) |
                                         (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf
                                                             : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) |
                                         (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

        ImGui::Text("Change colors of the lamp post lights");

        bool colorsChanged = false;

        auto mc_base_colors = mainController->light.get_base_colors();
        colorsChanged |= ImGui::ColorEdit3("Base Color 1", (float *) &mc_base_colors[0], misc_flags);
        colorsChanged |= ImGui::ColorEdit3("Base Color 2", (float *) &mc_base_colors[1], misc_flags);
        colorsChanged |= ImGui::ColorEdit3("Base Color 3", (float *) &mc_base_colors[2], misc_flags);
        colorsChanged |= ImGui::ColorEdit3("Base Color 4", (float *) &mc_base_colors[3], misc_flags);
        colorsChanged |= ImGui::ColorEdit3("Base Color 5", (float *) &mc_base_colors[4], misc_flags);

        ImGui::Spacing();
        ImGui::Text("Adjust brightness");

        const float MIN_BRIGHTNESS = 10.0f;
        const float MAX_BRIGHTNESS = 50.0f;

        auto mc_brightness = mainController->light.get_brightness();
        colorsChanged |= ImGui::SliderFloat("Brightness 1", &mc_brightness[0], MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        colorsChanged |= ImGui::SliderFloat("Brightness 2", &mc_brightness[1], MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        colorsChanged |= ImGui::SliderFloat("Brightness 3", &mc_brightness[2], MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        colorsChanged |= ImGui::SliderFloat("Brightness 4", &mc_brightness[3], MIN_BRIGHTNESS, MAX_BRIGHTNESS);
        colorsChanged |= ImGui::SliderFloat("Brightness 5", &mc_brightness[4], MIN_BRIGHTNESS, MAX_BRIGHTNESS);

        if (colorsChanged) {
            updateLampColors();
        }

        ImGui::Spacing();
        ImGui::Text("Final lamp colors (with brightness applied):");
        auto mc_lamp_colors = mainController->light.get_lamp_colors();
        for (int i = 0; i < 5; i++) {
            ImGui::ColorButton(("Final " + std::to_string(i + 1)).c_str(),
                               ImVec4(mc_lamp_colors[i].r, mc_lamp_colors[i].g, mc_lamp_colors[i].b, 1.0f),
                               ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_HDR, ImVec2(50, 20));
            ImGui::SameLine();
            ImGui::Text("Lamp %d: (%.1f, %.1f, %.1f)", i + 1, mc_lamp_colors[i].r, mc_lamp_colors[i].g,
                        mc_lamp_colors[i].b);
        }

        ImGui::End();

        graphics->end_gui();
    }

} // namespace app
