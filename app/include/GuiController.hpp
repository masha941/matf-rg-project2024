#ifndef GUICONTROLLER_HPP
#define GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>
#include <glm/glm.hpp>

namespace app {

    class GUIController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::GUIController";
        }

    private:
        void initialize() override;
        void draw() override;
        void poll_events() override;
        void update_lamp_colors();
        bool m_enabled = false;
    };

} // namespace app

#endif // GUICONTROLLER_HPP
