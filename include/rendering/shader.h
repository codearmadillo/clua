#pragma once
#include <vector>
#include <cstdint>

namespace Rendering {
    class Shader {
        public:
            ~Shader();
            Shader& attach(uint32_t type, const char* source, uint8_t count = 1, bool sourceIsFile = false);
            Shader& compile();
            Shader& bind();
            Shader& unbind();
        private:
            uint32_t m_program;
            std::vector<uint32_t> m_shaders;
    };
}