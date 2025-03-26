
#include <glad/glad.h>
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

        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), modelMatrices, GL_STATIC_DRAW);

        for (const auto &mesh: this->m_meshes) {
            unsigned int VAO = mesh.get_vao();
            glBindVertexArray(VAO);
            std::size_t vec4Size = sizeof(glm::vec4);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) 0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (1 * vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (3 * vec4Size));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }
    }
} // namespace engine::resources
