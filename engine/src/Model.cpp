
#include <glad/glad.h>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

#include <vector>

namespace engine::resources {

    void Model::draw(const Shader *shader) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.draw(shader);
        }
    }

    void Model::draw_instanced(const Shader *shader, int amount) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.draw_instanced(shader, amount);
        }
    }

    void Model::destroy() {
        for (auto &mesh: m_meshes) {
            mesh.destroy();
        }
    }

    void Model::prepare_instance_matrices(int amount, glm::mat4 *modelMatrices) {
        unsigned int buffer;

        CHECKED_GL_CALL(glGenBuffers, 1, &buffer);
        CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, buffer);
        CHECKED_GL_CALL(glBufferData, GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

        for (const auto &mesh: this->m_meshes) {
            unsigned int VAO = mesh.get_vao();
            glBindVertexArray(VAO);
            std::size_t vec4Size = sizeof(glm::vec4);

            CHECKED_GL_CALL(glEnableVertexAttribArray, 3);
            CHECKED_GL_CALL(glVertexAttribPointer, 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) 0);
            CHECKED_GL_CALL(glEnableVertexAttribArray, 4);
            CHECKED_GL_CALL(glVertexAttribPointer, 4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (1 * vec4Size));
            CHECKED_GL_CALL(glEnableVertexAttribArray, 5);
            CHECKED_GL_CALL(glVertexAttribPointer, 5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (2 * vec4Size));
            CHECKED_GL_CALL(glEnableVertexAttribArray, 6);
            CHECKED_GL_CALL(glVertexAttribPointer, 6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (3 * vec4Size));

            CHECKED_GL_CALL(glVertexAttribDivisor, 3, 1);
            CHECKED_GL_CALL(glVertexAttribDivisor, 4, 1);
            CHECKED_GL_CALL(glVertexAttribDivisor, 5, 1);
            CHECKED_GL_CALL(glVertexAttribDivisor, 6, 1);

            CHECKED_GL_CALL(glBindVertexArray, 0);
        }
    }
} // namespace engine::resources
