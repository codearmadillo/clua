#pragma once
#include <glm/vec3.hpp>
#include "GL/glew.h"

namespace Rendering {
    struct Vertex {
        glm::vec3 position;
        explicit Vertex(glm::vec3 c_position): position(c_position) {}
        static void setVertexAttribPointer() {
            // Position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
        }
    };
}