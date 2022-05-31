#pragma once

#include "rendering/element.h"
#include "rendering/vertex.h"
#include <vector>

namespace Rendering {
    enum BufferType {
        ELEMENT_BUFFER_OBJECT = 0,
        VERTEX_BUFFER_OBJECT = 1
    };
    /**
     * VAO
     */
    class ObjectBuffer {
        public:
            ObjectBuffer();
            ObjectBuffer& addElementBufferObject(std::vector<Element> elements);
            ObjectBuffer& addVertexBufferObject(std::vector<Vertex> vertices);
            ObjectBuffer& bind();
            ObjectBuffer& unbind();
            ObjectBuffer& compile();
            void draw();
        private:
            unsigned int m_vao{};
            unsigned int m_vbo{};
            unsigned int m_ebo{};
            unsigned int m_eboCount{0};
    };
}