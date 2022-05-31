#include <stdexcept>
#include <iostream>
#include "rendering/shader.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace Rendering {
    Shader& Shader::Attach(uint32_t type, const char* source, uint8_t count, bool sourceIsFile) {
        if(sourceIsFile) {
            throw std::runtime_error("ERROR::SHADER::ATTACH: File source is not supported");
        }

        /** Create shader */
        unsigned int shader = glCreateShader(type);
        int status;
        char error[512];

        /** Attach source and compile it */
        glShaderSource(shader, count, &source, nullptr);
        glCompileShader(shader);

        /** Error handling */
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if(!status) {
            glGetShaderInfoLog(shader, 512, nullptr, error);
            std::cerr << "ERR::SHADER::";
            switch(type) {
                case GL_VERTEX_SHADER:
                    std::cerr << "VERTEX";
                    break;
                case GL_FRAGMENT_SHADER:
                    std::cerr << "FRAGMENT";
                    break;
                default:
                    std::cout << "UNKNOWN";
            }
            std::cerr << "::COMPILATION_FAILED " << error << std::endl;
        }

        /** Store for cleanup later */
        m_shaders.push_back(shader);

        return *this;
    }
    Shader& Shader::Compile() {
        /** Attach shaders and link program */
        for (auto shader : m_shaders) {
            glAttachShader(m_program, shader);
        }

        /** Error handling */
        glLinkProgram(m_program);
        int status;
        char error[512];
        glGetProgramiv(m_program, GL_LINK_STATUS, &status);
        if(!status) {
            glGetProgramInfoLog(m_program, 512, nullptr, error);
            std::cerr << "ERROR::PROGRAM::LINKING_FAILED: " << error << std::endl;
        }

        /** Cleanup */
        for (auto shader : m_shaders) {
            glDeleteShader(shader);
        }

        return *this;
    }
    void Shader::Use() const {
        glUseProgram(m_program);
    }
}