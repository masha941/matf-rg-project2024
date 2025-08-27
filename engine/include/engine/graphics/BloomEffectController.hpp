#ifndef BLOOMEFFECTCONTROLLER_HPP
#define BLOOMEFFECTCONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace engine::graphics {
    class BloomEffectController final : public core::Controller {
    public:
        void render_bloom();
        void prepare_hdr();
        void finalize_bloom();
        void initialize();
        void hdr_bloom_setup();

    private:
        unsigned int m_pingpong_fbo[2];
        unsigned int m_pingpong_colorbuffers[2];
        unsigned int m_hdr_fbo;
        unsigned int m_color_buffers[2];
        float m_exposure                = 1.0f;
        bool m_bloom                    = true;
        unsigned int m_scr_width;
        unsigned int m_scr_height;
    };
} // namespace engine::graphics

#endif // BLOOMEFFECTCONTROLLER_HPP
