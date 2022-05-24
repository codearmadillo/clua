#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/vec4.hpp"

class Window {
    public:
        static Window& getInstance() {
            static Window instance;
            return instance;
        }
        void start();
        Window& setWindowTitle(const char* title);
        Window& setWindowSize(int width, int height);
        Window& setWindowFlags();
        Window& setWindowClearColor(const glm::vec4& color);
        double getDeltaTime() const;
    private:
        Window();
        ~Window();
        void terminate();
    private:
        GLFWwindow* m_window;
        int m_width { 800 };
        int m_height { 600 };
        std::string m_title { "Clua window" };
        glm::vec4 m_clearColor { 0.0f, 0.0f, 0.0f, 1.0f };
        double m_deltaTime;
        double m_lastFrameTime;
};