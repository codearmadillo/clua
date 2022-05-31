#include <stdexcept>
#include <iostream>
#include "foundation/rendering.h"

#include "GL/glew.h"
#include "rendering/vertex.h"

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
                    layout (location = 1) in vec4 aColor;

                    out vec4 vertexColor;

                    void main() {
                        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                        vertexColor = aColor;
                    }
                )";
        const char* fragmentSource = R"(
                    #version 330 core
                    in vec4 vertexColor;
                    out vec4 FragColor;
                    void main() {
                        FragColor = vertexColor;
                    }
                )";

        // Create program, compile and use it
        m_defaultShader
                .attach(GL_FRAGMENT_SHADER, fragmentSource)
                .attach(GL_VERTEX_SHADER, vertexSource)
                .compile()
                .bind();
    }
    void Module::setBindings() {

    }
}

/**
// Rendering test
    std::vector<Rendering::Element> elements = {
            Rendering::Element(0), Rendering::Element(1), Rendering::Element(3),
            Rendering::Element(1), Rendering::Element(2), Rendering::Element(3)
    };
    std::vector<Rendering::Vertex> vertices = {
            Rendering::Vertex({ 0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }),
            Rendering::Vertex({ 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }),
            Rendering::Vertex({ -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }),
            Rendering::Vertex({ -0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }),
    };

    Rendering::ObjectBuffer objectBuffer;
    objectBuffer
            .addVertexBufferObject(vertices)
            .addElementBufferObject(elements)
            .compile();


   objectBuffer.draw();
*/