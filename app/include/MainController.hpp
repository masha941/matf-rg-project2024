//
// Created by masa on 29/01/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {

    class MainController : public engine::core::Controller {

        void initialize() override;
        bool loop() override;
        void draw_temple();
        void draw() override;
        void begin_draw() override;
        void end_draw() override;
    public:
        std::string_view name() const override {
            return "app::MainController";
        }
    };

} // namespace app

#endif // MAINCONTROLLER_HPP
