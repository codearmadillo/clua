#include "rendering/object-buffer.h"

#include "GL/glew.h"

namespace Rendering {
    ObjectBuffer::ObjectBuffer() {
        glGenVertexArrays(1, &m_vao);
    }
    ObjectBuffer& ObjectBuffer::bind() {
        glBindVertexArray(m_vao);
        return *this;
    }
    ObjectBuffer& ObjectBuffer::unbind() {
        glBindVertexArray(0);
        return *this;
    }
    ObjectBuffer &ObjectBuffer::addElementBufferObject(std::vector<Element> elements) {
        bind();

        m_eboCount = elements.size();

        glGenBuffers(1, &m_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(Element), &elements.front(), GL_STATIC_DRAW);

        return unbind();
    }
    ObjectBuffer &ObjectBuffer::addVertexBufferObject(std::vector<Vertex> vertices) {
        bind();

        // Create buffer
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.front(), GL_STATIC_DRAW);

        // Attach attribute information
        Vertex::setVertexAttribPointer();

        return unbind();
    }
    ObjectBuffer& ObjectBuffer::compile() {
        glBindVertexArray(0);

        glDeleteBuffers(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);

        return *this;
    }
    void ObjectBuffer::draw() {
        bind();
        glDrawElements(GL_TRIANGLES, m_eboCount, GL_UNSIGNED_INT, (void*)nullptr);
        unbind();
    }
}