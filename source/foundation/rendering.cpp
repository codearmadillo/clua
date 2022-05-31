#include <stdexcept>
#include <iostream>
#include "foundation/rendering.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Rendering {
    void Module::onBeforeWindowStart() {
        // Compile shaders
        setShaders();
    }
    void Module::setShaders() {
        // Define shaders
        const char* vertexSource = R"(
                    #version 330 core
                    layout (location = 0) in vec3 aPos;
                    void main() {
                        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                    }
                )";
        const char* fragmentSource = R"(
                    #version 330 core
                    out vec4 FragColor;
                    void main() {
                        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
                    }
                )";

        // Create program, compile and use it
        m_defaultShader
                .Attach(GL_FRAGMENT_SHADER, fragmentSource)
                .Attach(GL_VERTEX_SHADER, vertexSource)
                .Compile()
                .Use();
    }
    void Module::setBindings() {
        std::cout << "set bindings\n";
    }
}