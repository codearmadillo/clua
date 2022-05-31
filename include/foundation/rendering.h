#pragma once
#include "rendering/shader.h"

namespace Rendering {
    class Module {
        public:
            static Module& getInstance() {
                static Module instance;
                return instance;
            }
            Module(Module const&) = delete;
            void operator=(Module const&) = delete;
            void setBindings();
            void onBeforeWindowStart();
        private:
            Module() = default;
            void setShaders();
        private:
            Shader m_defaultShader;
    };
}