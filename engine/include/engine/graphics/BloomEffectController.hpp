#ifndef BLOOMEFFECTCONTROLLER_HPP
#define BLOOMEFFECTCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace engine::graphics {
    class BloomEffectController final : public core::Controller {
    public:
        unsigned int pingpongFBO[2];
        unsigned int pingpongColorbuffers[2];
        unsigned int hdrFBO;
        unsigned int colorBuffers[2];
        float exposure                = 1.0f;
        bool bloom                    = true;
        const unsigned int SCR_WIDTH  = 1920;
        const unsigned int SCR_HEIGHT = 1080;
        void render_bloom();
        void hdr_bloom_setup();

    private:

    };
} // namespace engine::graphics

#endif // BLOOMEFFECTCONTROLLER_HPP
