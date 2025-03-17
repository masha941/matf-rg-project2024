//
// Created by masa on 14/03/25.
//
#include <../libs/glad/include/glad/glad.h>

#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/ResourcesController.hpp>
#include <spdlog/spdlog.h>
#include "engine/graphics/BloomEffect.hpp"

namespace engine::graphics {

    void BloomEffectController::hdr_bloom_setup() {
        CHECKED_GL_CALL(glGenFramebuffers, 1, &hdrFBO);
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, hdrFBO);
        CHECKED_GL_CALL(glGenTextures, 2, colorBuffers);

        for (unsigned int i = 0; i < 2; ++i) {
            CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, colorBuffers[i]);
            CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT,
                            nullptr);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                            colorBuffers[i], 0);
        }

        unsigned int rboDepth;
        CHECKED_GL_CALL(glGenRenderbuffers, 1, &rboDepth);
        CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, rboDepth);
        CHECKED_GL_CALL(glRenderbufferStorage, GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
        CHECKED_GL_CALL(glFramebufferRenderbuffer, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        CHECKED_GL_CALL(glDrawBuffers, 2, attachments);
        //if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //    spdlog::info("Framebuffer not complete!\n");
        //}

        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

        CHECKED_GL_CALL(glGenFramebuffers, 2, pingpongFBO);
        CHECKED_GL_CALL(glGenTextures, 2, pingpongColorbuffers);

        for (unsigned int i = 0; i < 2; ++i) {
            CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, pingpongFBO[i]);
            CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, pingpongColorbuffers[i]);
            CHECKED_GL_CALL(glTexImage2D, GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT,
                            nullptr);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            CHECKED_GL_CALL(glFramebufferTexture2D, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                            pingpongColorbuffers[i], 0);

            //if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            //    spdlog::info("Framebuffer not complete!\n");
            //}
        }
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Shader *shader = resources->shader("basic");
        engine::resources::Shader *blur_shader = resources->shader("blur");
        engine::resources::Shader *bloom_final = resources->shader("bloom_final");
        engine::resources::Shader *light       = resources->shader("light");

        shader->use();
        shader->set_int("texture_diffuse1", 0);
        blur_shader->use();
        blur_shader->set_int("image", 0);
        bloom_final->use();
        bloom_final->set_int("scene", 0);
        bloom_final->set_int("bloomBlur", 1);
    }

    void BloomEffectController::render_bloom() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader *blur_shader = resources->shader("blur");
        engine::resources::Shader *bloom_final = resources->shader("bloom_final");

        bool horizontal      = true;
        bool first_iteration = true;
        blur_shader->use();
        unsigned int amount = 10;
        for (unsigned int i = 0; i < amount; ++i) {
            CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            blur_shader->set_int("horizontal", horizontal);
            CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
            CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D,
                            first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);
            graphics->renderQuad();
            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

        CHECKED_GL_CALL(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bloom_final->use();
        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, colorBuffers[0]);
        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE1);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);

        bloom_final->set_int("bloom", 1);
        bloom_final->set_float("exposure", 1.0f);
        graphics->renderQuad();
    }

} // namespace engine::graphics
