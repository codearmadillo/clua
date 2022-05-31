#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "GL/glew.h"

namespace Rendering {
    struct Vertex {
        glm::vec3 position;
        glm::vec4 color;
        explicit Vertex(glm::vec3 c_position, glm::vec4 c_color): position(c_position), color(c_color) {}
        static void setVertexAttribPointer() {
            // Position
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
            glEnableVertexAttribArray(0);
            // Color
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        }
    };
}