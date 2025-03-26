#ifndef BLOOMEFFECTCONTROLLER_HPP
#define BLOOMEFFECTCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace engine::graphics {
    class BloomEffectController final : public core::Controller {
    public:
        void render_bloom();
        void prepare_hdr();
        void finalize_bloom();
        void hdr_bloom_setup();

    private:
        unsigned int m_pingpongFBO[2];
        unsigned int m_pingpongColorbuffers[2];
        unsigned int m_hdrFBO;
        unsigned int m_colorBuffers[2];
        float m_exposure                = 1.0f;
        bool m_bloom                    = true;
        const unsigned int m_SCR_WIDTH  = 1920;
        const unsigned int m_SCR_HEIGHT = 1080;
    };
} // namespace engine::graphics

#endif // BLOOMEFFECTCONTROLLER_HPP
