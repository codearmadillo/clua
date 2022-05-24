#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <map>

class Keyboard {
    public:
        static Keyboard& getInstance() {
            static Keyboard instance;
            return instance;
        }
        Keyboard(Keyboard const&)         = delete;
        void operator=(Keyboard const&)  = delete;
        void setBindings();
        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    private:
        Keyboard();
        ~Keyboard();
        void setKeyMap(int key, int scancode, int action);
        static const char* getKeyName(int key, int scancode);
        static bool getKeyState(const char* name);
        static void fireOnKeyDown(int key, int scancode);
        static void fireOnKeyUp(int key, int scancode);
    private:
        std::map<int, bool> m_keys;
};